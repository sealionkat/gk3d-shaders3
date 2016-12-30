#ifndef MESH_H
#define MESH_H


class Mesh
{
  public:
    std::vector<Vertex> vertices;
    std::vector<GLuint> indices;
    std::vector<Texture> textures;
    glm::vec3 color;

    Mesh(std::vector<Vertex> vertices, std::vector<GLuint> indices, glm::vec3 color)
    {
        std::cout << "Initializing color mesh" << std::endl;
        this->vertices = vertices;
        this->indices = indices;
        this->color = color;

        this->setupMesh();
    }

    Mesh(std::vector<Vertex> vertices, std::vector<GLuint> indices, std::vector<Texture> textures)
    {
        std::cout << "Initializing texture mesh" << std::endl;

        this->vertices = vertices;
        this->indices = indices;
        this->textures = textures;

        this->setupMesh();
    }

    void CalculateNormals()
    {
        for (auto i = std::begin(indices); i != std::end(indices); std::advance(i, 3))
        {
            glm::vec3 v[] = {vertices[i[0]].Position, vertices[i[1]].Position, vertices[i[2]].Position};
            glm::vec3 norm = glm::cross(v[1] - v[0], v[2] - v[0]);

            for (int j = 0; j < 3; ++j)
            {
                vertices[*(i + j)].Normal += norm;
            }
        }

        for (auto i = std::begin(vertices); i != std::end(vertices); std::advance(i, 1))
        {
            i->Normal = glm::normalize(i->Normal);
        }
    }

    void Draw(Shader shader)
    {
        //std::cout << "Drawing mesh" << std::endl;
        glBindVertexArray(this->VAO);
        glDrawElements(GL_TRIANGLES, this->indices.size(), GL_UNSIGNED_INT, 0);
        glBindVertexArray(0);
    }

    void setupMesh()
    {
        glGenVertexArrays(1, &this->VAO);
        glGenBuffers(1, &this->VBO);
        glGenBuffers(1, &this->EBO);

        glBindVertexArray(this->VAO); //enable VAO

        glBindBuffer(GL_ARRAY_BUFFER, this->VBO);
        glBufferData(GL_ARRAY_BUFFER, this->vertices.size() * sizeof(Vertex), &this->vertices[0], GL_STATIC_DRAW);

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->EBO);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, this->indices.size() * sizeof(GLuint), &this->indices[0], GL_STATIC_DRAW);

        glEnableVertexAttribArray(0); // position
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid *)offsetof(Vertex, Position));

        glEnableVertexAttribArray(1); // normal
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid *)offsetof(Vertex, Normal));

        glEnableVertexAttribArray(2); // texture coords
        glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid *)offsetof(Vertex, TexCoords));

        glBindVertexArray(0); // disable VAO
    }

    void setupSimpleMesh()
    {
        glGenVertexArrays(1, &this->VAO);
        glGenBuffers(1, &this->VBO);
        glGenBuffers(1, &this->EBO);

        glBindVertexArray(this->VAO); //enable VAO

        glBindBuffer(GL_ARRAY_BUFFER, this->VBO);
        glBufferData(GL_ARRAY_BUFFER, this->vertices.size() * sizeof(Vertex), &this->vertices[0], GL_STATIC_DRAW);

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->EBO);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, this->indices.size() * sizeof(GLuint), &this->indices[0], GL_STATIC_DRAW);

        glEnableVertexAttribArray(0); // position
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid *)offsetof(Vertex, Position));

        glBindVertexArray(0); // disable VAO
    }

  private:
    GLuint VBO, VAO, EBO;
};

#endif // !MESH_H