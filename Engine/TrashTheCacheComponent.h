#pragma once
#include "Component.h"
#include <imgui_plot.h>
#include <iostream>

class TrashTheCacheComponent final : public Component
{
public:
    TrashTheCacheComponent(GameObject* pOwner) : Component(pOwner) {}
public:
    struct Transform
    {
        float matrix[16] =
        {
            1,0,0,0,
            0,1,0,0,
            0,0,1,0,
            0,0,0,1
        };
    };

    struct GameObject3D
    {
        Transform local{};
        int id{};
    };

    struct GameObject3DAlt
    {
        Transform* local{};
        int id{};
    };

    enum class ContainerType
    {
        Integers,
        GameObject3D,
        GameObject3DAlt
    };

    virtual void Render() const override
    {
        ImGui::NewFrame();
        ImGui::Begin("Trash the Cache");

        if (ImGui::Button("Run TrashTheCache for Integers"))
        {
            TrashTheCache(ContainerType::Integers);
        }

        if (ImGui::Button("Run TrashTheCache for GameObject3D"))
        {
            TrashTheCache(ContainerType::GameObject3D);
        }

        if (ImGui::Button("Run TrashTheCache for GameObject3DAlt"))
        {
            TrashTheCache(ContainerType::GameObject3DAlt);
        }

        ImGui::ShowDemoWindow();

        ImGui::End();
        ImGui::EndFrame();
    } 
    

	void TrashTheCache(ContainerType containerType) const
	{
        std::vector<int> arrInt;
        std::vector<GameObject3D> arrGameObject3D;
        std::vector<GameObject3DAlt> arrGameObject3DAlt;

        switch (containerType)
        {
        case TrashTheCacheComponent::ContainerType::Integers:
            arrInt.resize(m_ArrSize);
            break;
        case TrashTheCacheComponent::ContainerType::GameObject3D:
            arrGameObject3D.resize(m_ArrSize);
            break;
        case TrashTheCacheComponent::ContainerType::GameObject3DAlt:
            arrGameObject3DAlt.resize(m_ArrSize);
            break;
        }

        std::cout.imbue(std::locale("fr_FR"));

        for (int stepsize{ 1 }; stepsize <= 1024; stepsize *= 2)
        {
            float totalElapsedTime = 0.0f;
            float minElapsedTime = std::numeric_limits<float>::max();
            float maxElapsedTime = 0.0f;

            for (int iteration = 0; iteration < m_NumIterations; ++iteration)
            {
                auto start = std::chrono::high_resolution_clock::now();


                switch (containerType)
                {
                case TrashTheCacheComponent::ContainerType::Integers:
                    for (int index{}; index < arrInt.size(); index += stepsize)
                    {
                        ++arrInt[index];
                    }
                    break;
                case TrashTheCacheComponent::ContainerType::GameObject3D:
                    for (int index{}; index < arrInt.size(); index += stepsize)
                    {
                        ++arrGameObject3D[index].id;
                    }
                    break;
                case TrashTheCacheComponent::ContainerType::GameObject3DAlt:
                    for (int index{}; index < arrInt.size(); index += stepsize)
                    {
                        ++arrGameObject3DAlt[index].id;
                    }                    
                    break;
                }

                auto elapsedSec = std::chrono::duration<float>(std::chrono::high_resolution_clock::now() - start).count();

                // Update min and max times
                minElapsedTime = std::min(minElapsedTime, elapsedSec);
                maxElapsedTime = std::max(maxElapsedTime, elapsedSec);

                totalElapsedTime += elapsedSec;
            }

            // Calculate and output the average time
            float averageElapsedTime = (totalElapsedTime - minElapsedTime - maxElapsedTime) / (m_NumIterations - 2);
            std::cout << std::setw(10) << std::to_string(stepsize) << "; " << std::setw(15) << averageElapsedTime << "\n";
        }
	}

private:

	const size_t m_ArrSize{ 100000 };
	const int m_NumIterations = 10;
};

