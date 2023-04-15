#include "ai_impl.h"

AIInterface::AIInterface(const char* token)
{
	create(token);
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

AIBase::AIBase(const char* token, const char* request, const char* fileName)
	:aiInterface{ token }, request_str{ request }, toConsole{ false }
{
}

void AIBase::sendRequest()
{
	aiInterface.send(type(), request_str.c_str());
}

void AIBase::showResponse()
{
	Buffer buffer = getResultData();
	int status = aiInterface.status();
	if (status == 0)
		doNormalOutput(buffer);
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
	std::cerr << errorMsg.data();
	exit(aiInterface.status());
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

ChatAI::ChatAI(const char* token, const char* request, const char* fileName)
	:AIBase(token, request, fileName)
{
	if (!openFile(fileName))
		exit(-1);
}

int ChatAI::type()
{
	return AI_TYPE_CHAT;
}

bool ChatAI::openFile(const char* fileName)
{
	outputFile.open(fileName);
	return outputFile.is_open();
}

bool ChatAI::isResultString()
{
	return true;
}

void ChatAI::doNormalOutput(const Buffer& data)
{
	if (toConsole)
		std::cout << data.data();
	else
		outputFile << data.data();
}

MathAI::MathAI(const char* token, const char* request, const char* fileName)
	:AIBase(token, request, fileName)
{
	if (!openFile(fileName))
		exit(-1);
}

int MathAI::type()
{
	return AI_TYPE_WOLFRAM;
}

bool MathAI::openFile(const char* fileName)
{
	outputFile.open(fileName, ios::binary);
	return outputFile.is_open();
}

void MathAI::doNormalOutput(const Buffer& data)
{
	outputFile.write(data.data(), data.size());
}

DrawAI::DrawAI(const char* token, const char* request, const char* fileName)
	:AIBase(token, request, fileName)
{
	if (!openFile(fileName))
		exit(-1);
}

int DrawAI::type()
{
	return AI_TYPE_DRAW;
}

bool DrawAI::openFile(const char* fileName)
{
	outputFile.open(fileName, ios::binary);
	return outputFile.is_open();
}

void DrawAI::doNormalOutput(const Buffer& data)
{
	outputFile.write(data.data(), data.size());
}
