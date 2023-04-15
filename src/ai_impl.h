#ifndef AI_IMPL_H
#define AI_IMPL_H

#include "ai.h"
#include <rjsjai.h>
#include <string>
#include <iostream>
#include <fstream>
#include <cstdlib>
#include <stdexcept>

using std::string;
using std::ofstream;
using std::ios;

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
	AIInterface(const char* token);
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
	string request_str;
protected:
	ofstream outputFile;
	bool toConsole;
public:
	AIBase(const char* token, const char* request)
		:aiInterface{ token }, request_str{ request }, toConsole{ true } {}
	AIBase(const char* token, const char* request, const char* fileName);
	void sendRequest() override;
	void showResponse() override;
protected:
	virtual int type() = 0;
	virtual bool openFile(const char* fileName) = 0;
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
	ChatAI(const char* token, const char* request, const char* fileName);
protected:
	int type() override;
	bool openFile(const char* fileName) override;
	bool isResultString() override;
	void doNormalOutput(const Buffer& data) override;
};

class MathAI
	:public AIBase
{
public:
	MathAI(const char* token, const char* request, const char* fileName);
protected:
	int type() override;
	bool openFile(const char* fileName) override;
	void doNormalOutput(const Buffer& data) override;
};

class DrawAI
	:public AIBase
{
public:
	DrawAI(const char* token, const char* request, const char* fileName);
protected:
	int type() override;
	bool openFile(const char* fileName) override;
	void doNormalOutput(const Buffer& data) override;
};

#endif
