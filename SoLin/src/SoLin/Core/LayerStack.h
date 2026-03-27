#pragma once

#include"Core.h"
#include"Layer.h"

#include<vector>

namespace SoLin {

	class SOLIN_API LayerStack
	{
	public:
		LayerStack();
		~LayerStack();

		void PushLayer(Layer* layer);		//普通图层入栈
		void PopLayer(Layer* layer);		//普通图层移除
		void PushOverLay(Layer* layer);		//覆盖图层处理
		void PopOverLay(Layer* layer);		//覆盖图层移除

		std::vector<Layer*>::iterator begin() { return m_Layers.begin(); }					//设计原因:
		std::vector<Layer*>::iterator end() { return m_Layers.end(); }						//当这个LayerStack对象作为for each loop的范围时
																							//要求LayerStack 这个容器具有begin()和end()成员函数，使其适应STL规范

	private:
		std::vector<Layer*> m_Layers;
		unsigned int m_LayerInsertIndex = 0;
	};
}

