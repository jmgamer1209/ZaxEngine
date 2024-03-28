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
	// ��������
	glGenTextures(1, texture);
	glBindTexture(GL_TEXTURE_2D, *texture);

	// ���û��Ʒ�ʽ�������ʾ s �� t ������Ϊ Repeat
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	// ���ù��˷�ʽ�������ʾ �Ŵ���Сʱ��������
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	stbi_set_flip_vertically_on_load(flip_vertically);  // �Ƿ���� y �ᷴת������һ���Ե�ͼƬ��������Ҫ�ģ����Ƕ���mesh�����uv�ģ�������Ҫ
	int width, height, nrChannels;
	unsigned char* data = stbi_load(path.c_str(), &width, &height, &nrChannels, 0);

	GLint format = GL_RGB;
	if (nrChannels == 4) format = GL_RGBA;

	// �����Ƿ���4�ı��������ö�ȡ���صĵ�λ
	if (nrChannels == 4 || (width%4 == 0 && height%4 == 0)) glPixelStorei(GL_UNPACK_ALIGNMENT, 4);   // 4�ֽڶ�ȡ
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

    // ����˳��̶�
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

        // �����Ƿ���4�ı��������ö�ȡ���صĵ�λ
        if (nrChannels == 4 || (width % 4 == 0 && height % 4 == 0)) glPixelStorei(GL_UNPACK_ALIGNMENT, 4);   // 4�ֽڶ�ȡ
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
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);   // ע�⻷�Ʒ�ʽ
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);  // �ر�� R ����

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
    viewRotation = glm::transpose(viewRotation); // ���������߼�ȡת�þ���

    glm::mat4 viewTranslation(1.0f);
    viewTranslation = glm::translate(viewTranslation, glm::vec3(-position.x, -position.y, -position.z));

    return viewRotation * viewTranslation;
}