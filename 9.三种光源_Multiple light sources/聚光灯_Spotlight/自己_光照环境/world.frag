#version 330 core//Ƭ����ɫ������

struct Material//������ʣ��Լ�������ɫ�����淴��ǿ�ȡ�ɢ��뾶
{
sampler2D  diffuse;  //�������������ɫ�����屾����ɫ��
sampler2D specular; //���淴���ǲ��Ǻ���
float shininess;//ɢ��뾶
};

struct Light //�������
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

in vec3 Normal;//������
in vec3 FragPos;//Ƭ��λ��
in vec2 TexCoords;

out vec4 color;
uniform vec3 cameraPos;
uniform Material material;
uniform Light light;

void main()
    {
	   // ����۹���յ�Ƭ���ϵĹ���
	     vec3 lightDir = normalize( light.position - FragPos);//�����ҵ�Ƭ���ϵķ���λ��
	     float theta = dot(lightDir,normalize(-light.direction));//����Ƭ������շ���ļнǵ�cosֵ
		 float intensity = clamp( (theta-light.outcutoff) / (light.cutoff-light.outcutoff), 0.0, 1.0);//��������׵��֮�䡢֮�ڵĹ���ǿ������

		//ȫ�ֹ���
	    vec3 ambient = light.ambient * vec3(texture(material.diffuse,TexCoords));//ȫ�ֹ���
		//���������
		vec3 norm = normalize(Normal);//����������λ��
		float diff = max(dot(norm,lightDir) , 0.0);//����������
		vec3 diffuse = diff * light.diffuse * vec3(texture(material.diffuse,TexCoords));//����������
		//���淴��
		vec3 viewDir = normalize(cameraPos - FragPos);//Ƭ��������ӽ����ɽǶ�
		vec3 reflectDir = reflect(-lightDir, norm);//���������Ĺ�Ƕ�
		float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess );//���淴�����ӣ�32������ķ���ֵ������ֵ���⻬�ȣ�Խ�߷�������Խǿ
		vec3 specular = vec3(texture(material.specular,TexCoords)) * spec * light.specular;//���淴������

		vec3 result = diffuse*intensity + ambient + specular*intensity;//���ս��
		color = vec4(result, 1.0f);
		

    }