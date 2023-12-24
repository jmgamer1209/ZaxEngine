#include "Utils.h"
#include "glad/glad.h"
#include "stb_image.h"


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

void Utils::Vec3ToArray(glm::vec3& vec3, float array[])
{
    array[0] = vec3[0];
    array[1] = vec3[1];
    array[2] = vec3[2];
}

glm::vec3 Utils::ArrayToVec3(float array[])
{
	return glm::vec3(array[0], array[1], array[2]);
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

	if (data)
	{
		glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}

	stbi_image_free(data);
}