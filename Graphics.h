#ifndef __OSHGUI_GRAPHICS_H__
#define __OSHGUI_GRAPHICS_H__

namespace OSHGui
{
	class Graphics
	{
	public:
		enum Types { DirectX, OpenGL };
		
	private:
		Types Type;
	
	public:
		Graphics(Types Type)
		{
			this->Type = Type;
		}
	};
}

#endif