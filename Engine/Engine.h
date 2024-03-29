#pragma once
#include <string>
#include <functional>


class Engine final
{
public:
	explicit Engine(const std::string& dataPath);
	~Engine();
	void Run(const std::function<void()>& load);

	Engine(const Engine& other) = delete;
	Engine(Engine&& other) = delete;
	Engine& operator=(const Engine& other) = delete;
	Engine& operator=(Engine&& other) = delete;
};