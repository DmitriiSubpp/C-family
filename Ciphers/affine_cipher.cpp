#include <iostream>

class Cipher
{
	private:
		
		std::string m_open_text;
		std::string m_cipher_text;
		
		int m_rev = 1;
		
		int gcd(int a, int b)
		{
			if (a < b) gcd(b, a);
			
			static int r;
			r = a % b;
			
			if (r != 0) gcd(b, r);
			if (b == 1) return 1;
		}
		
		
	public:
		
		Cipher(std::string text): m_open_text(text){}
		
		std::string get_opentext(){return m_open_text;}
		
		std::string& encryption(int a, int b)
		{
			if (!gcd(a, b)) exit(1);
			
			m_cipher_text = m_open_text;
			for (int i = 0; i < m_open_text.length(); i++)
			{
				if (m_open_text[i] >= 'a' && m_open_text[i] <= 'z')
					m_cipher_text[i] = (a * ((int)m_open_text[i]- 'a') + b) % 26 + 'a';
				else if (m_open_text[i] >= 'A' && m_open_text[i] <= 'Z')
					m_cipher_text[i] = (a * ((int)m_open_text[i]- 'A') + b) % 26 + 'A';
			}
			return m_cipher_text;
		}
		
		std::string& decryption(std::string cipher_text, int a, int b)
		{
			if (!gcd(a, b))
				exit(1);
			
			switch (a)
   			{
				case 1:  m_rev = 1;  break;
				case 3:  m_rev = 9;  break;
				case 5:  m_rev = 21; break;
				case 7:  m_rev = 15; break;
				case 9:  m_rev = 3;  break;
				case 11: m_rev = 19; break;
				case 15: m_rev = 7;  break;
				case 17: m_rev = 23; break;
				case 19: m_rev = 11; break;
				case 21: m_rev = 5;  break;
				case 23: m_rev = 17; break;
				case 25: m_rev = 25; break;
			}
			
			m_open_text = cipher_text;
			for (int i = 0; i < cipher_text.length(); i++)
			{
				if (cipher_text[i] >= 'a' && cipher_text[i] <= 'z')
					m_open_text[i] = m_rev * ((int)cipher_text[i]- 'a' + 26 - b) % 26 + 'a';
				else if (cipher_text[i] >= 'A' && cipher_text[i] <= 'Z')
					m_open_text[i] = m_rev * ((int)cipher_text[i]- 'A' + 26 - b) % 26 + 'A';
			}
			return m_open_text;
		}
		
		friend Cipher operator+ (const Cipher &t1,const Cipher &t2);
};

Cipher operator+ (const Cipher &t1,const Cipher &t2) {return Cipher(t1.m_open_text + t2.m_open_text) ;}

int main()
{
	Cipher open_text1("hello");
	Cipher open_text2(" world");
	Cipher text = open_text1 + open_text2;

	std::cout << text.get_opentext() << std::endl;
	
	std::string cipher_text = text.encryption(5, 7);
	std::cout << cipher_text << std::endl;
	
	std::string open_text = text.decryption(cipher_text, 5, 7);
	std::cout << open_text << std::endl;
}
