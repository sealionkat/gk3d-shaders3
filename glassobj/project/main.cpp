#include "Includes.h"

void key_callback(GLFWwindow *window, int key, int scancode, int action, int mode);
void mouse_callback(GLFWwindow *window, double xpos, double ypos);
void do_movement();
void draw_scene(glm::vec3 camPos, glm::vec3 camFront, glm::vec3 camUp, Shader shaderCube, Framebuffer buffer, Texture cubemap, int width, int height, GLfloat, GLuint cubeVAO);

const GLuint WIDTH = Settings::ScreenWidth, HEIGHT = Settings::ScreenHeight;

// Camera
glm::vec3 cameraPos = glm::vec3(0.0f, 2.0f, 6.5f);
glm::vec3 cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);
GLfloat yaw = -90.0f;
GLfloat pitch = 0.0f;
GLfloat lastX = WIDTH / 2.0;
GLfloat lastY = HEIGHT / 2.0;

// Deltatime
GLfloat deltaTime = 0.0f;
GLfloat lastFrame = 0.0f;

bool keys[1024];

int main()
{
    std::cout << "Starting GLFW context, OpenGL 3.3" << std::endl;

    glfwInit();

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, Settings::OpenGLMajorVersion);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, Settings::OpenGLMinorVersion);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

    GLFWwindow *window = glfwCreateWindow(WIDTH, HEIGHT, Settings::WindowTitle, nullptr, nullptr);
    if (window == nullptr)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);

    glfwSetKeyCallback(window, key_callback);
    glfwSetCursorPosCallback(window, mouse_callback);

    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    glewExperimental = GL_TRUE;
    if (glewInit() != GLEW_OK)
    {
        std::cout << "Failed to initialize GLEW" << std::endl;
        return -1;
    }

    int width, height;
    glfwGetFramebufferSize(window, &width, &height);
    glViewport(0, 0, width, height);

    glEnable(GL_DEPTH_TEST);

    std::cout << "Using " << glGetString(GL_VENDOR) << ", OpenGL " << glGetString(GL_VERSION) << ", Renderer " << glGetString(GL_RENDERER) << std::endl;

    std::cout << "Preperaing shaders..." << std::endl;
    const GLchar *vertexCubePath = "shaders/vertexCubeShader.glsl";
    const GLchar *fragmentCubePath = "shaders/fragmentCubeShader.glsl";
    const GLchar *vertexGlassPath = "shaders/vertexGlassShader.glsl";
    const GLchar *fragmentGlassPath = "shaders/fragmentGlassShader.glsl";

    Shader shaderCube(vertexCubePath, fragmentCubePath);
    Shader shaderGlass(vertexGlassPath, fragmentGlassPath);

    std::cout << "Init framebuffers for dynamic environmental mapping..." << std::endl;

    Framebuffer xPos = Framebuffer(width, height);
    Framebuffer xNeg = Framebuffer(width, height);
    Framebuffer yPos = Framebuffer(width, height);
    Framebuffer yNeg = Framebuffer(width, height);
    Framebuffer zPos = Framebuffer(width, height);
    Framebuffer zNeg = Framebuffer(width, height);

    std::cout << "Preparing cameras for dynamic environmental mapping..." << std::endl;

    glm::vec3 camMainPos = glm::vec3(0.0f, 0.0f, 0.0f);
    glm::vec3 camHorizontalUp = glm::vec3(0.0f, 1.0f, 0.0f);
    glm::vec3 camVerticalUp = glm::vec3(1.0f, 0.0f, 0.0f);

    glm::vec3 camXPosFront = glm::vec3(1.0f, 0.0f, 0.0f);
    glm::vec3 camXNegFront = glm::vec3(-1.0f, 0.0f, 0.0f);
    glm::vec3 camYPosFront = glm::vec3(0.0f, 1.0f, 0.0f);
    glm::vec3 camYNegFront = glm::vec3(0.0f, -1.0f, 0.0f);
    glm::vec3 camZPosFront = glm::vec3(0.0f, 0.0f, 1.0f);
    glm::vec3 camZNegFront = glm::vec3(0.0f, 0.0f, -1.0f);

    std::cout << "Creating cube" << std::endl;
    GLfloat cube[] = {
        8.0f, 8.0f, 8.0f, -1.0f, 0.0f, 0.0f, 0.0f, 0.0f, //right
        8.0f, 8.0f, -8.0f, -1.0f, 0.0f, 0.0f, 0.0f, 0.0f,
        8.0f, -8.0f, 8.0f, -1.0f, 0.0f, 0.0f, 0.0f, 0.0f,

        8.0f, -8.0f, 8.0f, -1.0f, 0.0f, 0.0f, 0.0f, 0.0f,
        8.0f, 8.0f, -8.0f, -1.0f, 0.0f, 0.0f, 0.0f, 0.0f,
        8.0f, -8.0f, -8.0f, -1.0f, 0.0f, 0.0f, 0.0f, 0.0f,

        -8.0f, -8.0f, 8.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, //left
        -8.0f, 8.0f, 8.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f,
        -8.0f, 8.0f, -8.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f,

        -8.0f, -8.0f, 8.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f,
        -8.0f, 8.0f, -8.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f,
        -8.0f, -8.0f, -8.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f,

        -8.0f, 8.0f, 8.0f, 0.0f, -1.0f, 0.0f, 0.0f, 0.0f, //up
        -8.0f, 8.0f, -8.0f, 0.0f, -1.0f, 0.0f, 0.0f, 0.0f,
        8.0f, 8.0f, -8.0f, 0.0f, -1.0f, 0.0f, 0.0f, 0.0f,

        8.0f, 8.0f, -8.0f, 0.0f, -1.0f, 0.0f, 0.0f, 0.0f,
        8.0f, 8.0f, 8.0f, 0.0f, -1.0f, 0.0f, 0.0f, 0.0f,
        -8.0f, 8.0f, 8.0f, 0.0f, -1.0f, 0.0f, 0.0f, 0.0f,

        -8.0f, -8.0f, 8.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, //down
        -8.0f, -8.0f, -8.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f,
        8.0f, -8.0f, -8.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f,

        -8.0f, -8.0f, 8.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f,
        8.0f, -8.0f, -8.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f,
        8.0f, -8.0f, 8.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f,

        -8.0f, -8.0f, -8.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, //back
        -8.0f, 8.0f, -8.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f,
        8.0f, 8.0f, -8.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f,

        8.0f, 8.0f, -8.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f,
        8.0f, -8.0f, -8.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f,
        -8.0f, -8.0f, -8.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f,

        -8.0f, -8.0f, 8.0f, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f, //front
        8.0f, 8.0f, 8.0f, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f,
        8.0f, -8.0f, 8.0f, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f,

        -8.0f, -8.0f, 8.0f, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f,
        8.0f, 8.0f, 8.0f, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f,
        -8.0f, 8.0f, 8.0f, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f

    };

    GLuint cubeVBO, cubeVAO;
    glGenVertexArrays(1, &cubeVAO);
    glGenBuffers(1, &cubeVBO);

    glBindVertexArray(cubeVAO);

    glBindBuffer(GL_ARRAY_BUFFER, cubeVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(cube), cube, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid *)0); //location 0 - position
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid *)(3 * sizeof(GLfloat))); //location 1 - normal
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid *)(6 * sizeof(GLfloat))); // location 2 - texture coords
    glEnableVertexAttribArray(2);

    glBindBuffer(GL_ARRAY_BUFFER, 0);

    glBindVertexArray(0);

    std::cout << "Creating wolf object..." << std::endl;
    Model *wolf = new Model("models/Wolf.obj");
    wolf->SetColor(glm::vec3(0.75f, 0.75f, 0.75f));

    std::cout << "Loading cubemap..." << std::endl;
    std::vector<const GLchar *> faces;
    faces.push_back("textures/SunSetRight2048.png");
    faces.push_back("textures/SunSetLeft2048.png");
    faces.push_back("textures/SunSetUp2048.png");
    faces.push_back("textures/SunSetDown2048.png");
    faces.push_back("textures/SunSetBack2048.png");
    faces.push_back("textures/SunSetFront2048.png");
    Texture cubemap = Texture();
    cubemap.loadCubemap(faces);

    // Show loop
    std::cout << "Starting main loop!" << std::endl;
    while (!glfwWindowShouldClose(window))
    {
        GLfloat currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        glfwPollEvents();
        do_movement();

        glBindBuffer(GL_FRAMEBUFFER, 0);

        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glm::mat4 model;
        glm::mat4 view;
        glm::mat4 projection;

        //glDepthMask(GL_FALSE);
        shaderCube.Use();

        model = glm::rotate(model, glm::radians(0.0f), glm::vec3(1.0f, 0.0f, 0.0f));
        view = glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);
        projection = glm::perspective(glm::radians(Settings::FOV), (float)width / (float)height, Settings::PerspectiveNear, Settings::PerspectiveFar);

        glUniformMatrix4fv(glGetUniformLocation(shaderCube.Program, Settings::modelMatrixLoc), 1, GL_FALSE, glm::value_ptr(model));
        glUniformMatrix4fv(glGetUniformLocation(shaderCube.Program, Settings::viewMatrixLoc), 1, GL_FALSE, glm::value_ptr(view));
        glUniformMatrix4fv(glGetUniformLocation(shaderCube.Program, Settings::projectionMatrixLoc), 1, GL_FALSE, glm::value_ptr(projection));

        glUniform3f(glGetUniformLocation(shaderCube.Program, Settings::viewPosLoc), cameraPos.x, cameraPos.y, cameraPos.z);

        cubemap.useCubemap(shaderCube);

        glBindVertexArray(cubeVAO);
        glDrawArrays(GL_TRIANGLES, 0, 36);
        glBindVertexArray(0);
        //glDepthMask(GL_TRUE);

        /* DRAWING ON FRAMEBUFFERS */

        /* framebuffer xPos */
        // xPos->BindBuffer();

        // shaderCube.Use();

        // model = glm::rotate(model, glm::radians(0.0f), glm::vec3(1.0f, 0.0f, 0.0f));
        // view = glm::lookAt(camMainPos, camMainPos + camXPosFront, camHorizontalUp);
        // projection = glm::perspective(glm::radians(Settings::FOV), (float)width / (float)height, Settings::PerspectiveNear, Settings::PerspectiveFar);

        // glUniformMatrix4fv(glGetUniformLocation(shaderCube.Program, Settings::modelMatrixLoc), 1, GL_FALSE, glm::value_ptr(model));
        // glUniformMatrix4fv(glGetUniformLocation(shaderCube.Program, Settings::viewMatrixLoc), 1, GL_FALSE, glm::value_ptr(view));
        // glUniformMatrix4fv(glGetUniformLocation(shaderCube.Program, Settings::projectionMatrixLoc), 1, GL_FALSE, glm::value_ptr(projection));

        // glUniform3f(glGetUniformLocation(shaderCube.Program, Settings::viewPosLoc), camMainPos.x, camMainPos.y, camMainPos.z);

        // cubemap->useCubemap(shaderCube);

        // glBindVertexArray(cubeVAO);
        // glDrawArrays(GL_TRIANGLES, 0, 36);
        // glBindVertexArray(0);

        // xPos->DetachBuffer();

        // /* framebuffer xNeg */

        // xNeg->BindBuffer();

        // shaderCube.Use();

        // model = glm::rotate(model, glm::radians(0.0f), glm::vec3(1.0f, 0.0f, 0.0f));
        // view = glm::lookAt(camMainPos, camMainPos + camXNegFront, camHorizontalUp);
        // projection = glm::perspective(glm::radians(Settings::FOV), (float)width / (float)height, Settings::PerspectiveNear, Settings::PerspectiveFar);

        // glUniformMatrix4fv(glGetUniformLocation(shaderCube.Program, Settings::modelMatrixLoc), 1, GL_FALSE, glm::value_ptr(model));
        // glUniformMatrix4fv(glGetUniformLocation(shaderCube.Program, Settings::viewMatrixLoc), 1, GL_FALSE, glm::value_ptr(view));
        // glUniformMatrix4fv(glGetUniformLocation(shaderCube.Program, Settings::projectionMatrixLoc), 1, GL_FALSE, glm::value_ptr(projection));

        // glUniform3f(glGetUniformLocation(shaderCube.Program, Settings::viewPosLoc), camMainPos.x, camMainPos.y, camMainPos.z);

        // cubemap->useCubemap(shaderCube);

        // glBindVertexArray(cubeVAO);
        // glDrawArrays(GL_TRIANGLES, 0, 36);
        // glBindVertexArray(0);

        // xNeg->DetachBuffer();

        // /* framebuffer yPos */

        // yPos->BindBuffer();

        // shaderCube.Use();

        // model = glm::rotate(model, glm::radians(0.0f), glm::vec3(1.0f, 0.0f, 0.0f));
        // view = glm::lookAt(camMainPos, camMainPos + camYPosFront, camVerticalUp);
        // projection = glm::perspective(glm::radians(Settings::FOV), (float)width / (float)height, Settings::PerspectiveNear, Settings::PerspectiveFar);

        // glUniformMatrix4fv(glGetUniformLocation(shaderCube.Program, Settings::modelMatrixLoc), 1, GL_FALSE, glm::value_ptr(model));
        // glUniformMatrix4fv(glGetUniformLocation(shaderCube.Program, Settings::viewMatrixLoc), 1, GL_FALSE, glm::value_ptr(view));
        // glUniformMatrix4fv(glGetUniformLocation(shaderCube.Program, Settings::projectionMatrixLoc), 1, GL_FALSE, glm::value_ptr(projection));

        // glUniform3f(glGetUniformLocation(shaderCube.Program, Settings::viewPosLoc), camMainPos.x, camMainPos.y, camMainPos.z);

        // cubemap->useCubemap(shaderCube);

        // glBindVertexArray(cubeVAO);
        // glDrawArrays(GL_TRIANGLES, 0, 36);
        // glBindVertexArray(0);

        // yPos->DetachBuffer();

        // /* framebuffer yNeg */

        // yNeg->BindBuffer();

        // shaderCube.Use();

        // model = glm::rotate(model, glm::radians(0.0f), glm::vec3(1.0f, 0.0f, 0.0f));
        // view = glm::lookAt(camMainPos, camMainPos + camYNegFront, camVerticalUp);
        // projection = glm::perspective(glm::radians(Settings::FOV), (float)width / (float)height, Settings::PerspectiveNear, Settings::PerspectiveFar);

        // glUniformMatrix4fv(glGetUniformLocation(shaderCube.Program, Settings::modelMatrixLoc), 1, GL_FALSE, glm::value_ptr(model));
        // glUniformMatrix4fv(glGetUniformLocation(shaderCube.Program, Settings::viewMatrixLoc), 1, GL_FALSE, glm::value_ptr(view));
        // glUniformMatrix4fv(glGetUniformLocation(shaderCube.Program, Settings::projectionMatrixLoc), 1, GL_FALSE, glm::value_ptr(projection));

        // glUniform3f(glGetUniformLocation(shaderCube.Program, Settings::viewPosLoc), camMainPos.x, camMainPos.y, camMainPos.z);

        // cubemap->useCubemap(shaderCube);

        // glBindVertexArray(cubeVAO);
        // glDrawArrays(GL_TRIANGLES, 0, 36);
        // glBindVertexArray(0);

        // yNeg->DetachBuffer();

        // /* framebuffer zPos */

        // zPos->BindBuffer();

        // shaderCube.Use();

        // model = glm::rotate(model, glm::radians(0.0f), glm::vec3(1.0f, 0.0f, 0.0f));
        // view = glm::lookAt(camMainPos, camMainPos + camZPosFront, camHorizontalUp);
        // projection = glm::perspective(glm::radians(Settings::FOV), (float)width / (float)height, Settings::PerspectiveNear, Settings::PerspectiveFar);

        // glUniformMatrix4fv(glGetUniformLocation(shaderCube.Program, Settings::modelMatrixLoc), 1, GL_FALSE, glm::value_ptr(model));
        // glUniformMatrix4fv(glGetUniformLocation(shaderCube.Program, Settings::viewMatrixLoc), 1, GL_FALSE, glm::value_ptr(view));
        // glUniformMatrix4fv(glGetUniformLocation(shaderCube.Program, Settings::projectionMatrixLoc), 1, GL_FALSE, glm::value_ptr(projection));

        // glUniform3f(glGetUniformLocation(shaderCube.Program, Settings::viewPosLoc), camMainPos.x, camMainPos.y, camMainPos.z);

        // cubemap->useCubemap(shaderCube);

        // glBindVertexArray(cubeVAO);
        // glDrawArrays(GL_TRIANGLES, 0, 36);
        // glBindVertexArray(0);

        // zPos->DetachBuffer();

        // /* framebuffer zNeg */

        // zNeg->BindBuffer();

        // shaderCube.Use();

        // model = glm::rotate(model, glm::radians(0.0f), glm::vec3(1.0f, 0.0f, 0.0f));
        // view = glm::lookAt(camMainPos, camMainPos + camZNegFront, camHorizontalUp);
        // projection = glm::perspective(glm::radians(Settings::FOV), (float)width / (float)height, Settings::PerspectiveNear, Settings::PerspectiveFar);

        // glUniformMatrix4fv(glGetUniformLocation(shaderCube.Program, Settings::modelMatrixLoc), 1, GL_FALSE, glm::value_ptr(model));
        // glUniformMatrix4fv(glGetUniformLocation(shaderCube.Program, Settings::viewMatrixLoc), 1, GL_FALSE, glm::value_ptr(view));
        // glUniformMatrix4fv(glGetUniformLocation(shaderCube.Program, Settings::projectionMatrixLoc), 1, GL_FALSE, glm::value_ptr(projection));

        // glUniform3f(glGetUniformLocation(shaderCube.Program, Settings::viewPosLoc), camMainPos.x, camMainPos.y, camMainPos.z);

        // cubemap->useCubemap(shaderCube);

        // glBindVertexArray(cubeVAO);
        // glDrawArrays(GL_TRIANGLES, 0, 36);
        // glBindVertexArray(0);

        // zNeg->DetachBuffer();

        /* /DRAWING ON FRAMEBUFFERS */

        /* creating cubemap for dynamic environmental mapping */

        /* / */
        //glDepthMask(GL_TRUE);
        shaderGlass.Use();

        view = glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);
        projection = glm::perspective(glm::radians(Settings::FOV), (float)width / (float)height, Settings::PerspectiveNear, Settings::PerspectiveFar);

        glUniformMatrix4fv(glGetUniformLocation(shaderGlass.Program, Settings::viewMatrixLoc), 1, GL_FALSE, glm::value_ptr(view));
        glUniformMatrix4fv(glGetUniformLocation(shaderGlass.Program, Settings::projectionMatrixLoc), 1, GL_FALSE, glm::value_ptr(projection));
        glUniform3f(glGetUniformLocation(shaderGlass.Program, Settings::viewPosLoc), cameraPos.x, cameraPos.y, cameraPos.z);

        glm::mat4 scaledModel;
        scaledModel = glm::scale(scaledModel, glm::vec3(0.02f, 0.02f, 0.02f));
        glUniformMatrix4fv(glGetUniformLocation(shaderGlass.Program, Settings::modelMatrixLoc), 1, GL_FALSE, glm::value_ptr(scaledModel));

        cubemap.useCubemap(shaderGlass);

        wolf->Draw(shaderGlass);

        glfwSwapBuffers(window);
    }

    std::cout << "Cleaning..." << std::endl;
    glDeleteVertexArrays(1, &cubeVAO);
    glDeleteBuffers(1, &cubeVBO);

    std::cout << "Terminating application..." << std::endl;
    glfwTerminate();

    return 0;
}

void key_callback(GLFWwindow *window, int key, int scancode, int action, int mode)
{
    // std::cout << "Key: " << key << std::endl;
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
    {
        glfwSetWindowShouldClose(window, GL_TRUE);
    }
    if (key >= 0 && key < 1024)
    {
        if (action == GLFW_PRESS)
        {
            keys[key] = true;
        }
        else if (action == GLFW_RELEASE)
        {
            keys[key] = false;
        }
    }
}

void do_movement()
{
    GLfloat cameraSpeed = Settings::CameraSpeed * deltaTime;
    if (keys[Settings::UpKey])
    {
        cameraPos += cameraSpeed * cameraUp;
    }
    if (keys[Settings::DownKey])
    {
        cameraPos -= cameraSpeed * cameraUp;
    }
    if (keys[Settings::ForwardKey])
    {
        cameraPos += cameraSpeed * cameraFront;
    }
    if (keys[Settings::BackwardKey])
    {
        cameraPos -= cameraSpeed * cameraFront;
    }
    if (keys[Settings::LeftKey])
    {
        cameraPos -= glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
    }
    if (keys[Settings::RightKey])
    {
        cameraPos += glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
    }
}

bool firstMouse = true;
void mouse_callback(GLFWwindow *window, double xpos, double ypos)
{
    if (firstMouse)
    {
        lastX = xpos;
        lastY = ypos;
        firstMouse = false;
    }

    GLfloat xoffset = xpos - lastX;
    GLfloat yoffset = lastY - ypos;
    lastX = xpos;
    lastY = ypos;

    GLfloat sensitivity = Settings::MouseSensitivity;
    xoffset *= sensitivity;
    yoffset *= sensitivity;

    yaw += xoffset;
    pitch += yoffset;

    if (pitch > 89.0f)
    {
        pitch = 89.0f;
    }
    if (pitch < -89.0f)
    {
        pitch = -89.0f;
    }

    glm::vec3 front;
    front.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
    front.y = sin(glm::radians(pitch));
    front.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
    cameraFront = glm::normalize(front);
}

void draw_scene(glm::vec3 camPos, glm::vec3 camFront, glm::vec3 camUp, Shader shaderCube, Framebuffer buffer, Texture cubemap, int width, int height, GLuint cubeVAO)
{
    buffer.BindBuffer();

    shaderCube.Use();

    glm::mat4 model;
    glm::mat4 view;
    glm::mat4 projection;

    model = glm::rotate(model, glm::radians(0.0f), glm::vec3(1.0f, 0.0f, 0.0f));
    view = glm::lookAt(camPos, camPos + camFront, camUp);
    projection = glm::perspective(glm::radians(Settings::FOV), (float)width / (float)height, Settings::PerspectiveNear, Settings::PerspectiveFar);

    glUniformMatrix4fv(glGetUniformLocation(shaderCube.Program, Settings::modelMatrixLoc), 1, GL_FALSE, glm::value_ptr(model));
    glUniformMatrix4fv(glGetUniformLocation(shaderCube.Program, Settings::viewMatrixLoc), 1, GL_FALSE, glm::value_ptr(view));
    glUniformMatrix4fv(glGetUniformLocation(shaderCube.Program, Settings::projectionMatrixLoc), 1, GL_FALSE, glm::value_ptr(projection));

    glUniform3f(glGetUniformLocation(shaderCube.Program, Settings::viewPosLoc), camPos.x, camPos.y, camPos.z);

    cubemap.useCubemap(shaderCube);

    glBindVertexArray(cubeVAO);
    glDrawArrays(GL_TRIANGLES, 0, 36);
    glBindVertexArray(0);

    buffer.DetachBuffer();
}
