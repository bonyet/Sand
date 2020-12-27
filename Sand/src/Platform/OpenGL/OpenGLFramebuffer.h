#pragma once

#include "Sand/Renderer/Framebuffer.h"

namespace Sand 
{

	class OpenGLFramebuffer : public Framebuffer
	{
	public:
		OpenGLFramebuffer(const FramebufferSpecification& spec);
		virtual ~OpenGLFramebuffer();

		void Invalidate();

		virtual void Bind() override;
		virtual void Unbind() override;

		virtual void Resize(uint32_t width, uint32_t height) override;

		virtual const FramebufferSpecification& GetSpecification() const override { return m_Specification; };
	
		virtual uint32_t GetColorAttachmentRendererID() const override { return m_ColorAttachment; }
	private:
		uint32_t m_RendererID;
		uint32_t m_ColorAttachment, m_DepthAttachment;
		uint32_t m_IDAttachment;
		FramebufferSpecification m_Specification;
	};

}