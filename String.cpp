#include "String.h"
#include <iostream>
#include <assert.h>

using namespace std;

namespace TheUltimateString
{
		String::String()
		{
			pData=NULL;
			elementsNum=0;
		}

		String::String(const String& string)
		{
			pData=NULL;
			copy(*this,string);
		}

		
		String::String(const char c, unsigned int times)
		{
			elementsNum=times;

			if(elementsNum==0)
			{
				pData=NULL;
				return;
			}

			pData=new char[elementsNum+1];
			for(unsigned int i=0;i<elementsNum;i++)
			{
				pData[i]=c;
			}
			pData[elementsNum]='\0';
		}

		//conversion constructor
		String::String(const char* str)
		{
			elementsNum = strlen(str);
			pData = new char[elementsNum + 1];
			for (int i = 0; i < elementsNum; i++)
			{
				pData[i] = str[i];
			}

		}


		void String::print(ostream& os)
		{
			for(unsigned int i=0;i<elementsNum;i++)
			{
				os<<pData[i];
			}
		}

		String String::concatenate(const String & string1, const String & string2)
		{
			String s;
			s.elementsNum=string1.elementsNum+string2.elementsNum;

			if(s.elementsNum==0)
			{
				s.pData=NULL;
				return s;
			}

			s.pData=new char[s.elementsNum+1];

			for(unsigned int i=0;i<string1.elementsNum;i++)
			{
				s.pData[i]=string1.pData[i];
			}
			for(unsigned int j=string1.elementsNum;j<s.elementsNum;j++)
			{
				s.pData[j]=string2.pData[j-string1.elementsNum];
			}

			s.pData[s.elementsNum]='\0';
			return s;
		}

		bool String::compare(const String & string1, const String & string2)
		{
			if(string1.elementsNum!=string2.elementsNum)return false;

			for(unsigned int i=0;i<string1.elementsNum;i++)
			{
				if(string1.pData[i]!=string2.pData[i]) return false;
			}
			return true;
		}

		void String::copy(String &string1, const String &string2)
		{
			delete string1.pData;	
			string1.elementsNum=string2.elementsNum;
			if(string1.elementsNum==0)
			{
				string1.pData=NULL;
				return;
			}
			string1.pData=new char[string1.elementsNum+1];
			for(unsigned int i=0;i<string1.elementsNum;i++)
			{
				string1.pData[i]=string2.pData[i];
			}
			string1.pData[string1.elementsNum]='\0';
		}

		bool String::operator==(const String& theOther)const
		{
			//using strcmp which throws zero if they match
			if (strcmp(pData, theOther.pData) == 0)
			{
				return true;
			}
			//else false
			else
			{
				return false;
			}
		}

		String String::operator+(const String& theOther)const
		{

			return concatenate(*this, theOther);

		}

		const String& String::operator+=(const String& theOther)
		{
			return *this = concatenate(*this, theOther);
		}

		const String& String::operator=(const String& theOther)
		{
			copy(*this, theOther);
			return *this;
		}
		




}