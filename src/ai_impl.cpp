#include "ai_impl.h"

AIInterface::AIInterface(const string& token)
{
	create(token.c_str());
	//aiInterface = nullptr;
}

AIInterface::AIInterface(AIInterface&& rhs)
{
	aiInterface = rhs.aiInterface;
	rhs.aiInterface = nullptr;
}

AIInterface& AIInterface::operator=(AIInterface&& rhs)
{
	if (aiInterface)
		ai_free(aiInterface);
	aiInterface = rhs.aiInterface;
	rhs.aiInterface = nullptr;
	return *this;
}

void AIInterface::create(const char* token)
{
	aiInterface = ai_create(token);
}

int AIInterface::send(int type, const char* prompt)
{
	return ai_send(aiInterface, type, prompt);
}

int AIInterface::status()
{
	return ai_status(aiInterface);
}

int AIInterface::resultBytesCount(bool isString)
{
	return ai_result(aiInterface, nullptr) + (isString ? 1 : 0);
}

Buffer AIInterface::result(bool isString)
{
	Buffer buffer;
	ai_result(aiInterface, buffer.getWriteable(resultBytesCount(isString)));
	if (isString)
		buffer[buffer.size() - 1] = '\0';
	return buffer;
}

AIInterface::~AIInterface()
{
	if (aiInterface)
		ai_free(aiInterface);
}

void AIBase::sendRequest()
{
	aiInterface.send(type(), request.c_str());
}

void AIBase::showResponse()
{
	Buffer buffer = getResultData();
	int status = aiInterface.status();
	if (status == 0)
	{
		if(fileName)
			openFile(*fileName);
		doNormalOutput(buffer);
	}
	else
		doErrorOutput(buffer);
}

bool AIBase::isResultString()
{
	return aiInterface.status() != 0;
}

Buffer AIBase::getResultData()
{
	return aiInterface.result(isResultString());
}

void AIBase::doErrorOutput(const Buffer& errorMsg)
{
	errorExit(string(errorMsg.data(), errorMsg.size()), aiInterface.status());
}

Buffer::Buffer(const Buffer& rhs)
	:length{ rhs.length }
{
	buffer = new char[length];
	memcpy(buffer, rhs.buffer, length);
}

Buffer::Buffer(Buffer&& rhs)
	:length{ rhs.length }, buffer{ rhs.buffer }
{
	rhs.buffer = nullptr;
}

Buffer& Buffer::operator=(const Buffer& rhs)
{
	if (this == &rhs)
		return *this;
	length = rhs.length;
	delete buffer;
	buffer = new char[length];
	memcpy(buffer, rhs.buffer, length);
	return *this;
}

Buffer& Buffer::operator=(Buffer&& rhs)
{
	buffer = rhs.buffer;
	length = rhs.length;
	rhs.buffer = nullptr;
	return *this;
}

int Buffer::size() const
{
	return length;
}

char* Buffer::getWriteable(int minSize)
{
	if (minSize > length)
		reAllocate(minSize);
	else
		length = minSize;
	return buffer;
}

const char* Buffer::data() const
{
	return buffer;
}

char& Buffer::operator[](int index) const
{
	if (index >= length || index < 0)
		throw std::out_of_range("Index out of range");
	return buffer[index];
}

void Buffer::reAllocate(int newSize)
{
	if (buffer)
		delete buffer;
	buffer = new char[newSize];
	length = newSize;
}

void ChatAI::attachFile(const string& fileName)
{
	this->fileName = fileName;
}

int ChatAI::type()
{
	return AI_TYPE_CHAT;
}

void ChatAI::openFile(const string& fileName)
{
	outputFile.open(fileName);
	if (!outputFile.is_open())
		errorExit("Open file failed", FILE_ERROR);
}

bool ChatAI::isResultString()
{
	return true;
}

void ChatAI::doNormalOutput(const Buffer& data)
{
	if (!fileName)
		std::cout << data.data();
	else
		outputFile << data.data();
}

int MathAI::type()
{
	return AI_TYPE_WOLFRAM;
}

void MathAI::openFile(const string& fileName)
{
	outputFile.open(fileName, ios::binary);
	if(!outputFile.is_open())
		errorExit("Open file failed", FILE_ERROR);
}

void MathAI::doNormalOutput(const Buffer& data)
{
	outputFile.write(data.data(), data.size());
}

int DrawAI::type()
{
	return AI_TYPE_DRAW;
}

void DrawAI::openFile(const string& fileName)
{
	outputFile.open(fileName, ios::binary);
	if (!outputFile.is_open())
		errorExit("Open file failed", FILE_ERROR);
}

void DrawAI::doNormalOutput(const Buffer& data)
{
	outputFile.write(data.data(), data.size());
}
