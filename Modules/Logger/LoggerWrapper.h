﻿#pragma once
#include "../Logger/spdlog/spdlog.h"

class LoggerWrapper
{
public:
	static LoggerWrapper* Instance();
	LoggerWrapper();
public:
	//主要的接口
	template<typename Fmt, typename... Args>
	void LogInfo(Fmt&& fmt, Args&&... args);

	template<typename Fmt, typename... Args>
	void LogDebug(Fmt&& fmt, Args&&... args);

	template<typename Fmt, typename... Args>
	void LogWarning(Fmt&& fmt, Args&&... args);

	template<typename Fmt, typename... Args>
	void LogError(Fmt&& fmt, Args&&... args);
private:
	static std::unique_ptr<LoggerWrapper> m_instance;
	void Init();
};

template<typename Fmt, typename... Args>
void LoggerWrapper::LogError(Fmt&& fmt, Args&&... args)
{
	spdlog::get("console")->error(fmt, args...);
	spdlog::get("file_logger")->error(fmt, args...);
}

template<typename Fmt, typename... Args>
void LoggerWrapper::LogWarning(Fmt&& fmt, Args&&... args)
{
	spdlog::get("console")->warn(fmt, args...);
	spdlog::get("file_logger")->warn(fmt, args...);
}

template<typename Fmt, typename... Args>
void LoggerWrapper::LogDebug(Fmt&& fmt, Args&&... args)
{
	spdlog::get("console")->debug(fmt, args...);
	spdlog::get("file_logger")->debug(fmt, args...);
}

template<typename Fmt, typename... Args>
void LoggerWrapper::LogInfo(Fmt&& fmt, Args&&... args)
{
	spdlog::get("console")->info(fmt, args...);
	spdlog::get("file_logger")->info(fmt, args...);
}

#define  Logger LoggerWrapper::Instance()

#define LogDebug(fmt, ...) Logger->LogDebug(fmt, ##__VA_ARGS__)
#define LogInfo(fmt, ...) Logger->LogInfo(fmt, ##__VA_ARGS__)
#define LogWarn(fmt, ...) Logger->LogWarn(fmt, ##__VA_ARGS__)
#define LogError(fmt, ...) Logger->LogError(fmt, ##__VA_ARGS__)
