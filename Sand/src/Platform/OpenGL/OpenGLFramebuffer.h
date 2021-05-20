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
		virtual int ReadPixel(uint32_t attachmentIndex, int x, int y) override;

		virtual const FramebufferSpecification& GetSpecification() const override { return m_Specification; };
	
		virtual uint32_t GetColorAttachmentRendererID(uint32_t index = 0) const override { 
			SAND_CORE_ASSERT(index < m_ColorAttachments.size(), "Invalid color attachment index.");  
			return m_ColorAttachments[index]; 
		}
	private:
		uint32_t m_Handle;
		FramebufferSpecification m_Specification;

		std::vector<FramebufferTextureSpecification> m_ColorAttachmentSpecs;
		FramebufferTextureSpecification m_DepthAttachmentSpec;

		std::vector<uint32_t> m_ColorAttachments;
		uint32_t m_DepthAttachment;
	};

}