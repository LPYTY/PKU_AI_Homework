#ifndef AI_IMPL_H
#define AI_IMPL_H

#include "./ai.h"
#include "./error.h"

#include <rjsjai.h>
#include <string>
#include <iostream>
#include <fstream>
#include <cstdlib>
#include <stdexcept>
#include <optional>

using std::string;
using std::ofstream;
using std::ios;
using std::optional;

constexpr char myToken[] = "fa501f56-34ac-410e-90f0-deeca8b0629c";

class Buffer
{
	char* buffer;
	int length;
public:
	Buffer()
		:buffer{ nullptr }, length{ 0 } {}
	Buffer(const Buffer& rhs);
	Buffer(Buffer&& rhs);
	Buffer& operator=(const Buffer& rhs);
	Buffer& operator=(Buffer&& rhs);
	int size() const;
	char* getWriteable(int minSize);
	const char* data() const;
	char& operator[](int index) const;
private:
	void reAllocate(int newSize);
};

class AIInterface
{
	RJSJAI* aiInterface;
public:
	AIInterface(const string& token);
	AIInterface(const AIInterface&) = delete;
	AIInterface(AIInterface&& rhs);
	AIInterface& operator=(const AIInterface&) = delete;
	AIInterface& operator=(AIInterface&& rhs);
	void create(const char* token);
	int send(int type, const char* prompt);
	int status();
	int resultBytesCount(bool isString);
	Buffer result(bool isString);
	~AIInterface();
};

class AIBase
	:public AI
{
	AIInterface aiInterface;
	string request;
protected:
	optional<string> fileName;
	ofstream outputFile;
	bool toConsole;
public:
	AIBase(const char* token, const string& request)
		:aiInterface{ token }, request{ request } {}
	AIBase(const char* token, const string& request, const string& fileName)
		:aiInterface{ token }, request{ request }, fileName{ fileName } {}
	void sendRequest() override;
	void showResponse() override;
protected:
	virtual int type() = 0;
	virtual void openFile(const string& fileName) = 0;
	virtual bool isResultString();
	virtual Buffer getResultData();
	virtual void doNormalOutput(const Buffer& data) = 0;
	virtual void doErrorOutput(const Buffer& errorMsg);
};

class ChatAI
	:public AIBase
{
public:
	using AIBase::AIBase;
	void attachFile(const string& fileName);
protected:
	int type() override;
	void openFile(const string& fileName) override;
	bool isResultString() override;
	void doNormalOutput(const Buffer& data) override;
};

class MathAI
	:public AIBase
{
public:
	using AIBase::AIBase;
protected:
	int type() override;
	void openFile(const string& fileName) override;
	void doNormalOutput(const Buffer& data) override;
};

class DrawAI
	:public AIBase
{
public:
	using AIBase::AIBase;
protected:
	int type() override;
	void openFile(const string& fileName) override;
	void doNormalOutput(const Buffer& data) override;
};

#endif
