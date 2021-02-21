#pragma once

#include "Sand/Renderer/Texture.h"
#include <glad/glad.h>

namespace Sand 
{

	class OpenGLTexture2D : public Texture2D
	{
	public:
		OpenGLTexture2D(uint32_t width, uint32_t height);
		OpenGLTexture2D(const std::string& path);
		virtual ~OpenGLTexture2D();

		virtual uint32_t GetWidth() const override { return m_Width;  }
		virtual uint32_t GetHeight() const override { return m_Height; }
		
		virtual void SetData(void* data, uint32_t size) override;
		virtual void SetWidth(uint32_t width) override;
		virtual void SetHeight(uint32_t height) override;

		virtual void Bind(uint32_t slot = 0) const override;

		virtual bool operator==(const Texture& other) const override {
			return m_RendererID == ((OpenGLTexture2D&)other).m_RendererID;
		}

		virtual uint32_t GetID() const override { return m_RendererID; }

		virtual const std::string GetPath() const override { return m_Path; }
	private:
		std::string m_Path;
		uint32_t m_Width, m_Height;
		uint32_t m_RendererID;
		GLenum m_InternalFormat, m_DataFormat;
	};

}