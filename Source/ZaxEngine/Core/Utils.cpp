#include "Utils.h"
#include "glad/gl.h"
#include "stb_image.h"
#include <string>
#include <vector>
#include "Core/Debug.h"


string Utils::WString2String(const wstring& ws)
{
    _bstr_t t = ws.c_str();
    char* pchar = (char*)t;
    string result = pchar;
    return result;
}

wstring Utils::String2Wstring(const string& s)
{
    _bstr_t t = s.c_str();
    wchar_t* pwchar = (wchar_t*)t;
    wstring result = pwchar;
    return result;
}

void Utils::LoadTexture(unsigned int *texture, string& path, bool flip_vertically)
{
	// 加载纹理
	glGenTextures(1, texture);
	glBindTexture(GL_TEXTURE_2D, *texture);

	// 设置环绕方式，下面表示 s 和 t 都设置为 Repeat
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	// 设置过滤方式，下面表示 放大缩小时都是线性
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	stbi_set_flip_vertically_on_load(flip_vertically);  // 是否进行 y 轴反转，对于一般性的图片，都是需要的，但是对于mesh顶点带uv的，并不需要
	int width, height, nrChannels;
	unsigned char* data = stbi_load(path.c_str(), &width, &height, &nrChannels, 0);

	GLint format = GL_RGB;
	if (nrChannels == 4) format = GL_RGBA;

	// 根据是否是4的倍数，设置读取像素的单位
	if (nrChannels == 4 || (width%4 == 0 && height%4 == 0)) glPixelStorei(GL_UNPACK_ALIGNMENT, 4);   // 4字节读取
	else glPixelStorei(GL_UNPACK_ALIGNMENT, 1); 

	if (data)
	{
		glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}

	stbi_image_free(data);
}

void Utils::LoadCubeMap(unsigned int* texture, const string& folderPath)
{
	glGenTextures(1, texture);
	glBindTexture(GL_TEXTURE_CUBE_MAP, *texture);

    // 加载顺序固定
    vector<std::string> faces
    {
        "right.jpg",
        "left.jpg",
        "top.jpg",
        "bottom.jpg",
        "back.jpg",
        "front.jpg"
    };

    int width, height, nrChannels;
    for (unsigned int i = 0; i < faces.size(); i++)
    {
        unsigned char* data = stbi_load((folderPath + faces[i]).c_str(), &width, &height, &nrChannels, 0);

        // 根据是否是4的倍数，设置读取像素的单位
        if (nrChannels == 4 || (width % 4 == 0 && height % 4 == 0)) glPixelStorei(GL_UNPACK_ALIGNMENT, 4);   // 4字节读取
        else glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

        if (data)
        {
            glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i,
                0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data
            );
            stbi_image_free(data);
        }
        else
        {
            Debug::Log({ "Cubemap texture failed to load at path: " , faces[i] });
            stbi_image_free(data);
        }
    }
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);   // 注意环绕方式
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);  // 特别的 R 方向

    glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
}

glm::mat4 Utils::GetViewMatrix(const Transform& transform)
{
    glm::mat4 viewRotation(1.0f);
    auto position = transform.position;
    auto rotation = transform.rotation;

    viewRotation = glm::rotate(viewRotation, glm::radians(rotation.y), glm::vec3(0, 1, 0));
    viewRotation = glm::rotate(viewRotation, glm::radians(rotation.x), glm::vec3(1, 0, 0));
    viewRotation = glm::rotate(viewRotation, glm::radians(rotation.z), glm::vec3(0, 0, 1));
    viewRotation = glm::transpose(viewRotation); // 根据运算逻辑取转置矩阵

    glm::mat4 viewTranslation(1.0f);
    viewTranslation = glm::translate(viewTranslation, glm::vec3(-position.x, -position.y, -position.z));

    return viewRotation * viewTranslation;
}

std::string Utils::GetExeDirectory() {
	std::vector<char> exePath(MAX_PATH);
	DWORD size = GetModuleFileNameA(NULL, exePath.data(), exePath.size());
	if (size == 0) {
		// 处理错误情况
		return "";
	}
 
	// 查找最后一个反斜杠
	for (int i = size - 1; i >= 0; --i) {
		if (exePath[i] == '\\') {
			exePath[i] = '\0'; // 结束标记
			return std::string(exePath.data());
		}
	}
 
	// 如果没有找到反斜杠，则返回空字符串
	return "";
}

std::string Utils::GetDefaultProjectPath() {
	auto path = Utils::GetExeDirectory();
	int count = 1;
	int index = 0;
	for (index = path.length(); index>= 0; index--)
	{
		if (path[index] == '\\')
		{
			count++;
		}
		if (count > 3)
		{
			break;
		}
	}
	return path.substr(0,index);
}