#version 330 core//Ƭ����ɫ������

in GS_OUT{
	vec3 Normal;
	vec3 FragPos;
	vec2 TexCoords;
}fs_in;
out vec4 color;

struct Material//������ʣ��Լ�������ɫ�����淴��ǿ�ȡ�ɢ��뾶
{
sampler2D  texture_diffuse1;  //�������������ɫ�����屾����ɫ��
sampler2D texture_specular1; //���淴���ǲ��Ǻ���
float shininess;//ɢ��뾶
};

struct DirLight //ƽ�й������
{
vec3 direction;//����
vec3 ambient;
vec3 diffuse;
vec3 specular;
};

struct PointLight
{
vec3 position;
vec3 ambient;
vec3 diffuse;
vec3 specular;
float Kc;
float Kl;
float Kq;

};

uniform vec3 cameraPos;
uniform Material material;
uniform DirLight dirlight;
uniform PointLight pointLights;


//���ռ��㺯��/////////////////
vec3 CalcDirLight(DirLight light,Material imaterial, vec3 norm, vec3 viewDir)//ƽ�й���㺯��
{
    //ȫ�ֹ���
    vec3 ambient = light.ambient * vec3(texture(imaterial.texture_diffuse1,fs_in.TexCoords));//ȫ�ֹ���
	//���������
	vec3 lightDir = normalize( -light.direction);
	float diff = max(dot(norm,lightDir) , 0.0);//����������
	vec3 diffuse = diff * light.diffuse * vec3(texture(imaterial.texture_diffuse1,fs_in.TexCoords));//����������
	//���淴��
	vec3 reflectDir = reflect(-lightDir, norm);//���������Ĺ�Ƕ�
	float spec = pow(max(dot(viewDir, reflectDir), 0.0), imaterial.shininess );//���淴�����ӣ�32������ķ���ֵ������ֵ���⻬�ȣ�Խ�߷�������Խǿ
	vec3 specular =  spec * light.specular * vec3(texture(imaterial.texture_specular1,fs_in.TexCoords));//���淴������ 
	return(diffuse + ambient + specular);
}

vec3 CalcPointLight(PointLight light,Material imaterial, vec3 fragPos, vec3 norm, vec3 viewDir)
{
   //ȫ�ֹ���
    vec3 ambient = light.ambient * vec3(texture(imaterial.texture_diffuse1,fs_in.TexCoords));//ȫ�ֹ���
	//���������
	vec3 lightDir = normalize(light.position - fragPos);
	float diff = max(dot(norm,lightDir) , 0.0);//����������
	vec3 diffuse = diff * light.diffuse * vec3(texture(imaterial.texture_diffuse1,fs_in.TexCoords));//����������
	//���淴��
	vec3 reflectDir = reflect(-lightDir, norm);//���������Ĺ�Ƕ�
	float spec = pow(max(dot(viewDir, reflectDir), 0.0), imaterial.shininess );//���淴�����ӣ�32������ķ���ֵ������ֵ���⻬�ȣ�Խ�߷�������Խǿ
	vec3 specular =  vec3(texture(imaterial.texture_specular1,fs_in.TexCoords)) * spec * light.specular;//���淴������
	//˥����
	float distance = length(light.position - fragPos);
	float attenuation = 1.0/(light.Kc + light.Kl*distance + light.Kq*distance*distance);

	return((diffuse + ambient + specular)*attenuation);
}

void main()
    {
	vec3 result;
	vec3 norm = normalize(fs_in.Normal);//����������λ��
	vec3 viewDir = normalize(cameraPos - fs_in.FragPos);//Ƭ��������ӽ����ɽǶ�
	result = CalcDirLight(dirlight,material, norm, viewDir );
	result += CalcPointLight(pointLights,material,fs_in.FragPos, norm, viewDir);

    color = vec4(result, 1.0f);
	
    }