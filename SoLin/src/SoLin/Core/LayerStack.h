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
                                                                                            //设计原因:
        std::vector<Layer*>::iterator begin() { return m_Layers.begin(); }					//当这个LayerStack对象作为for each loop的范围时
        std::vector<Layer*>::iterator end() { return m_Layers.end(); }						//要求LayerStack 这个容器具有begin()和end()成员函数，使其适应STL规范																					
        std::vector<Layer*>::const_iterator begin() const { return m_Layers.begin(); }
        std::vector<Layer*>::const_iterator end() const { return m_Layers.end(); }

        std::vector<Layer*>::reverse_iterator rbegin() { return m_Layers.rbegin(); }
        std::vector<Layer*>::reverse_iterator rend() { return m_Layers.rend(); }
        std::vector<Layer*>::const_reverse_iterator rbegin() const { return m_Layers.rbegin(); }
        std::vector<Layer*>::const_reverse_iterator rend()   const { return m_Layers.rbegin(); }

	private:
		std::vector<Layer*> m_Layers;
		unsigned int m_LayerInsertIndex = 0;
	};
}

