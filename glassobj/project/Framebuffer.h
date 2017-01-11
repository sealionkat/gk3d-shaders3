#ifndef FRAMEBUFFER_H
#define FRAMEBUFFER_H

class Framebuffer
{
  public:
    Framebuffer(GLuint screenWidth, GLuint screenHeight)
    {
        width = screenWidth;
        height = screenHeight;
        textureBytes = std::vector<GLubyte>(width * height * 4);
    }

    void InitTextureFramebuffer()
    {
        glGenFramebuffers(1, &fbo);
        glBindFramebuffer(GL_FRAMEBUFFER, fbo);

        //texture
        glGenTextures(1, &texture);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, texture, 0);

        glGenRenderbuffers(1, &rbo);
        glBindRenderbuffer(GL_RENDERBUFFER, rbo);
        glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, width, height);
        glBindRenderbuffer(GL_RENDERBUFFER, 0);

        glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, rbo);

        if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
        {
            std::cout << "Framebuffer is not complete!" << std::endl;
        }

        glBindFramebuffer(GL_FRAMEBUFFER, 0);
    }

    void BindBuffer()
    {
        glBindFramebuffer(GL_FRAMEBUFFER, fbo);
    }

    void UseBuffer()
    {
        glBindFramebuffer(GL_READ_FRAMEBUFFER, fbo);
        glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
        //glBlitFramebuffer(0, 0, width, height, 0, 0, width, height, GL_COLOR_BUFFER_BIT, GL_LINEAR);
    }

    void DetachBuffer()
    {
        //glReadPixels(0, 0, width, height, GL_RGB, GL_UNSIGNED_BYTE, textureBytes.data());
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
    }

    std::vector<GLubyte> getTexturePixels()
    {
        return textureBytes;
    }

  private:
    GLuint fbo;
    GLuint rbo;
    GLuint width;
    GLuint height;

    GLuint texture;
    std::vector<GLubyte> textureBytes;
};

#endif // !FRAMEBUFFER_H
