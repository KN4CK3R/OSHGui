#ifndef __OSHGUI_TAG_H__
#define __OSHGUI_TAG_H__

namespace OSHGui
{
	template <typename T> class Tag
	{
	public:
		Tag(const T& data)
		{
			this->data = data;
		}
		//---------------------------------------------------------------------------
		T& GetData()
		{
			return data;
		}
		//---------------------------------------------------------------------------
		
	private:
		T data;
	};
}

#endif