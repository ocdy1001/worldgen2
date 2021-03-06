#include "fps_camera.h"

FpsCamera::FpsCamera(int kup, int kdown, int kfor, int kback, int kleft, int kright){
    this->kup = kup;
    this->kdown = kdown;
    this->kfor = kfor;
    this->kback = kback;
    this->kleft = kleft;
    this->kright = kright;
}

void FpsCamera::input(GLFWwindow *window, float elaps, float xpos, float ypos){
    auto xzdir = camdir;
    xzdir.y = 0.0f;
    xzdir = glm::normalize(xzdir);
    auto xzri = camri;
    xzri.y = 0.0f;
    xzri = glm::normalize(xzri);
    auto up = glm::vec3(0.0f,1.0f,0.0f);
    if(glfwGetKey(window, kfor) == GLFW_PRESS)
        campos += xzdir * elaps * move_sens;
    if(glfwGetKey(window, kback) == GLFW_PRESS)
        campos -= xzdir * elaps * move_sens;
    if(glfwGetKey(window, kleft) == GLFW_PRESS)
        campos -= xzri * elaps * move_sens;
    if(glfwGetKey(window, kright) == GLFW_PRESS)
        campos += xzri * elaps * move_sens;
    if(glfwGetKey(window, kup) == GLFW_PRESS)
        campos += up * elaps * vert_sens;
    if(glfwGetKey(window, kdown) == GLFW_PRESS)
        campos -= up * elaps * vert_sens;
    if(firstmouse){
        lastx = xpos;
        lasty = ypos;
        firstmouse = false;
    }
    float dx = xpos - lastx;
    float dy = lasty - ypos;
    lastx = xpos;
    lasty = ypos;
    yaw += dx * mouse_sens;
    pitch += dy * mouse_sens;

    if(pitch > 89.99f)
        pitch = 89.99f;
    if(pitch < -89.99f)
        pitch = -89.99f;
}

void FpsCamera::apply_vp(Shader *shader, bool usepos){
    glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f);
    camdir.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
    camdir.y = sin(glm::radians(pitch));
    camdir.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
    camdir = glm::normalize(camdir);
    camri = glm::normalize(glm::cross(camdir, up));
    camup = glm::normalize(glm::cross(camri, camdir));
    glm::mat4 view = glm::lookAt(campos, campos + camdir, camup);
    glm::mat4 proj = glm::perspective(glm::radians(fov), aspect, near, far);
    if(!usepos){
        view = glm::mat4(glm::mat3(view));
    }

    shader->set_mat4("view", view);
    shader->set_mat4("proj", proj);
}
