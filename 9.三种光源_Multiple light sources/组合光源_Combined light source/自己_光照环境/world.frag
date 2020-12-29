#version 330 core//Ƭ����ɫ������

in vec3 Normal;//������
in vec3 FragPos;//Ƭ��λ��
in vec2 TexCoords;
out vec4 color;

struct Material//������ʣ��Լ�������ɫ�����淴��ǿ�ȡ�ɢ��뾶
{
sampler2D  diffuse;  //�������������ɫ�����屾����ɫ��
sampler2D specular; //���淴���ǲ��Ǻ���
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

struct ConeLight //�ֵ�������
{
vec3 position;
vec3 ambient;
vec3 diffuse;
vec3 specular;
//�۹������
vec3 direction;
float cutoff;//��׵���нǵ�����ֵ
float outcutoff;//��׵��
};

uniform vec3 cameraPos;
uniform Material material;
uniform DirLight dirlight;
uniform ConeLight conelight;
uniform PointLight pointLights[4];


//���ռ��㺯��/////////////////
vec3 CalcDirLight(DirLight light,Material imaterial, vec3 norm, vec3 viewDir)//ƽ�й���㺯��
{
    //ȫ�ֹ���
    vec3 ambient = light.ambient * vec3(texture(imaterial.diffuse,TexCoords));//ȫ�ֹ���
	//���������
	vec3 lightDir = normalize( -light.direction);
	float diff = max(dot(norm,lightDir) , 0.0);//����������
	vec3 diffuse = diff * light.diffuse * vec3(texture(imaterial.diffuse,TexCoords));//����������
	//���淴��
	vec3 reflectDir = reflect(-lightDir, norm);//���������Ĺ�Ƕ�
	float spec = pow(max(dot(viewDir, reflectDir), 0.0), imaterial.shininess );//���淴�����ӣ�32������ķ���ֵ������ֵ���⻬�ȣ�Խ�߷�������Խǿ
	vec3 specular = vec3(texture(imaterial.specular,TexCoords)) * spec * light.specular;//���淴������
	return(diffuse + ambient + specular);
}

vec3 CalcPointLight(PointLight light,Material imaterial, vec3 fragPos, vec3 norm, vec3 viewDir)
{
   //ȫ�ֹ���
    vec3 ambient = light.ambient * vec3(texture(imaterial.diffuse,TexCoords));//ȫ�ֹ���
	//���������
	vec3 lightDir = normalize(light.position - fragPos);
	float diff = max(dot(norm,lightDir) , 0.0);//����������
	vec3 diffuse = diff * light.diffuse * vec3(texture(imaterial.diffuse,TexCoords));//����������
	//���淴��
	vec3 reflectDir = reflect(-lightDir, norm);//���������Ĺ�Ƕ�
	float spec = pow(max(dot(viewDir, reflectDir), 0.0), imaterial.shininess );//���淴�����ӣ�32������ķ���ֵ������ֵ���⻬�ȣ�Խ�߷�������Խǿ
	vec3 specular = vec3(texture(imaterial.specular,TexCoords)) * spec * light.specular;//���淴������
	//˥����
	float distance = length(light.position - fragPos);
	float attenuation = 1.0/(light.Kc + light.Kl*distance + light.Kq*distance*distance);

	return((diffuse + ambient + specular)*attenuation);
}

vec3 CalcConeLight(ConeLight light,Material imaterial, vec3 fragPos, vec3 norm, vec3 viewDir)
{
  vec3 ambient = light.ambient * vec3(texture(imaterial.diffuse,TexCoords));//ȫ�ֹ���
	//���������
	vec3 lightDir = normalize(light.position - fragPos);
	float diff = max(dot(norm,lightDir) , 0.0);//����������
	vec3 diffuse = diff * light.diffuse * vec3(texture(imaterial.diffuse,TexCoords));//����������
	//���淴��
	vec3 reflectDir = reflect(-lightDir, norm);//���������Ĺ�Ƕ�
	float spec = pow(max(dot(viewDir, reflectDir), 0.0), imaterial.shininess );//���淴�����ӣ�32������ķ���ֵ������ֵ���⻬�ȣ�Խ�߷�������Խǿ
	vec3 specular = vec3(texture(imaterial.specular,TexCoords)) * spec * light.specular;//���淴������

	float theta = dot(lightDir,normalize(-light.direction));//����Ƭ������շ���ļнǵ�cosֵ
	float intensity = clamp( (theta-light.outcutoff) / (light.cutoff-light.outcutoff), 0.0, 1.0);//��������׵��֮�䡢֮�ڵĹ���ǿ������

	return(diffuse*intensity + ambient + specular*intensity);
}
void main()
    {
	vec3 result;
	vec3 norm = normalize(Normal);//����������λ��
	vec3 viewDir = normalize(cameraPos - FragPos);//Ƭ��������ӽ����ɽǶ�
	result = CalcDirLight(dirlight,material, norm, viewDir );
	for(int i = 0; i < 4; i++)
	{
	result += CalcPointLight(pointLights[i],material,FragPos, norm, viewDir);
	}
	result += CalcConeLight(conelight,material,FragPos,norm,viewDir);
	
    color = vec4(result, 1.0f);
    }