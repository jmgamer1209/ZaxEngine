#include "Utils.h"

#include <fstream>

#include "glad/gl.h"
#include "stb_image.h"
#include <string>
#include <vector>

#include "Application.h"
#include "Core/Debug.h"


char* Utils::WChar2Char(const wchar_t* wchar)
{
	char* m_char;    
	int len= WideCharToMultiByte( CP_ACP ,0, wchar, wcslen(wchar), NULL,0, NULL ,NULL );    
	m_char= new char[len+1];     
	WideCharToMultiByte( CP_ACP, 0, wchar, wcslen(wchar), m_char, len, NULL ,NULL );     
	m_char[len]= '\0';     
	return m_char;
}

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

Texture Utils::LoadTexture(string& path, bool flip_vertically) 
{
    unsigned int id;
    LoadTexture(&id, path, flip_vertically);
    Texture t(TextureType::Texture2D, id);
    return t;
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

boost::json::object Utils::LoadJsonFile(const boost::filesystem::path& path)
{
	ifstream ifstream;
	ifstream.open(path.string());
	boost::json::object obj = boost::json::parse(ifstream).as_object();
	ifstream.close();
	return obj;
}

boost::filesystem::path Utils::GetExeDirectory() {
    WCHAR path[MAX_PATH];
    GetModuleFileName(NULL, path, MAX_PATH);
    boost::filesystem::path result(path);
    return result.parent_path();
}