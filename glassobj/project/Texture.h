#ifndef TEXTURE_H
#define TEXTURE_H

class Texture
{
  public:
    void loadFromFile(const char *fileName, bool noRepeatS, bool noRepeatT)
    {
        glGenTextures(1, &texture);
        glBindTexture(GL_TEXTURE_2D, texture);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, noRepeatS ? GL_CLAMP_TO_BORDER : GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, noRepeatT ? GL_CLAMP_TO_BORDER : GL_REPEAT);

        if (noRepeatS || noRepeatT)
        {
            GLfloat borderColor[] = {0.0f, 0.0f, 0.0f, 0.0f}; //black and transparent
            glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);
        }

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        unsigned char *image = SOIL_load_image(fileName, &width, &height, 0, SOIL_LOAD_RGBA);

        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image);
        glGenerateMipmap(GL_TEXTURE_2D);

        SOIL_free_image_data(image);

        glBindTexture(GL_TEXTURE_2D, 0);
    }


    void loadCubemap(std::vector<const GLchar *> faces)
    {
        glGenTextures(1, &texture);
        glActiveTexture(GL_TEXTURE0);

        unsigned char *image;

        glBindTexture(GL_TEXTURE_CUBE_MAP, texture);

        for (GLuint i = 0; i < faces.size(); ++i)
        {
            image = SOIL_load_image(faces[i], &width, &height, 0, SOIL_LOAD_RGB);

            glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
        }

        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

        glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
    }

    void loadCubemap(const std::vector<std::vector<GLubyte> > &framebufferTextures)
    {
        glGenTextures(1, &texture);
        glActiveTexture(GL_TEXTURE0);

        glBindTexture(GL_TEXTURE_CUBE_MAP, texture);

        for (GLuint i = 0; i < framebufferTextures.size(); ++i)
        {
            glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, framebufferTextures[i].data());
        }

        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

        glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
    }

    void use(Shader shader, int num)
    {
        if (num >= 0 && num < Settings::TexturesCount)
        {
            glActiveTexture(GL_TEXTURE0 + num);
            glBindTexture(GL_TEXTURE_2D, texture);
            std::string name = std::string(Settings::texturesLoc) + std::string("[") + std::to_string(num) + std::string("]");
            glUniform1i(glGetUniformLocation(shader.Program, name.c_str()), num);
        }
    }

    void useCubemap(Shader shader)
    {
        glBindTexture(GL_TEXTURE_CUBE_MAP, texture);
    }

  private:
    GLuint texture;
    int width;
    int height;
    GLuint id;
    std::string type;
};

#endif // !TEXTURE_H
