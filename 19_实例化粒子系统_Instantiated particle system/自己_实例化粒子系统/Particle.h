#pragma once
#ifndef PARTICLE_CLASS_H
#define PARTICLE_CLASS_H 

#include <glm/glm.hpp>
class Particale {
private:
	glm::vec3 m_initposition;//初始位置
	glm::vec3 m_v0;//初速度
	float m_density;//密度
	float m_quality;//质量
	int m_life;//生命值
	float m_inittime;//创建时间

public:
	Particale(glm::vec3 m_initposition = glm::vec3(0.0, 0.0, 0.0), glm::vec3 m_v0 = glm::vec3(1.0, 0.0, 0.0), int m_life = 10, float m_inittime = 0.0, float m_quality = 1.0, float m_density = 1.0)
	{
		this->m_density = m_density;
		this->m_inittime = m_inittime;
		this->m_v0 = m_v0;
		this->m_life = m_life;
		this->m_quality = m_quality;
		this->m_initposition = m_initposition;
	}
	void ReduceLife(int size) //生命值减少
	{
		m_life -= size;
	}
	bool IsAlive()//判断粒子是否存活
	{
		if (m_life > 0)
			return true;
		else
			return false;
	}
	glm::vec3 GetOffset(glm::vec3 a, float nowtime )//关键公式，获得粒子的位移量(以后可以改写)
	{
		glm::vec3 offset;
		float t = nowtime - m_inittime;
		offset = m_v0 * t  -  (float)0.5 * a * t * t ; //位移公式
		return offset;
	}
	glm::vec3 GetInitPosition()
	{
		return m_initposition;
	}
};
#endif