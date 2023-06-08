#include <iostream>
#include <fstream>
#include <string>

using namespace std;

struct Article
{

	string Categorys = "", Article_Date = "", Article_Audience = "", Article_Title = "", Contact_Name = "", Contact_Email = "", Article_Body = "";

};

string Last_Characters(string String, int Total_Characters)
{

	if (String.length() > Total_Characters) return String.substr(String.length() - Total_Characters, String.length() - 1);
	else return String;

}

void Replace_All(std::string& String, std::string const& Find, std::string const& Replace)
{

	std::string Temp;
	std::size_t Position = 0;
	std::size_t Previous_Position;

	Temp.reserve(String.size());

	while (true)
	{

		Previous_Position = Position;

		Position = String.find(Find, Position);

		if (Position == std::string::npos)break;

		Temp.append(String, Previous_Position, Position - Previous_Position);

		Temp += Replace;
		Position += Find.size();

	}

	Temp.append(String, Previous_Position, String.size() - Previous_Position);
	String.swap(Temp);

}

string Read_Paragraph(ifstream& File)
{

	string Paragraph = "", Line = "", Last_Char = "0";
	bool Extracted = false;

	while (!Extracted)
	{

		getline(File, Line);

		Paragraph += Line + "\n";

		if (Line != "")
		{

			Last_Char = Last_Characters(Line, 2);

			if ((Last_Char == ".\"" || Last_Char == "\"") && Last_Char != "\"\"")Extracted = true;

		}

	}

	return Paragraph;

}

void Get_Category(string& Category)
{

	if (Category == "Chair's Message") Category = "1";
	else if (Category == "Director's Message")Category = "2";
	else if (Category == "Fun Stuff")Category = "3";
	else if (Category == "General")Category = "4";
	else if (Category == "Health")Category = "5";
	else if (Category == "Nutrition")Category = "6";
	else if (Category == "Places to go")Category = "7";
	else if (Category == "Policies and Procedures")Category = "8";
	else if (Category == "Safe Schools")Category = "9";
	else if (Category == "Seasonal")Category = "10";
	else if (Category == "Tips and Tools")Category = "11";
	else if (Category == "Weather/Environment")Category = "12";
	else if (Category == "Web Site News")Category = "13";

}

void Replace_Special_Characters(string& Sentence)
{

	Replace_All(Sentence, "\"", "");
	Replace_All(Sentence, "\"\"", "\"");
	Replace_All(Sentence, "\'", "\'\'");

}

void Clean_Article_Props(Article& Article)
{

	Replace_Special_Characters(Article.Article_Title);
	Replace_Special_Characters(Article.Article_Body);

	Get_Category(Article.Categorys);

}

Article Read_Article(ifstream& File)
{

	std::string Line = "";
	Article Article;

	if (getline(File, Line, ',')) Article.Categorys = Line;
	if (getline(File, Line, ',')) Article.Article_Date = Line;
	if (getline(File, Line, ',')) Article.Article_Audience = Line;
	if (getline(File, Line, ',')) Article.Article_Title = Line;
	if (getline(File, Line, ',')) Article.Contact_Name = Line;
	if (getline(File, Line, ',')) Article.Contact_Email = Line;
	Article.Article_Body = Read_Paragraph(File);

	Clean_Article_Props(Article);

	return Article;

}

string Form_Query(Article Article)
{

	string SQL_Query = "";

	SQL_Query += "INSERT INTO dbo.Webteam_NewsYouCanUse_Article (Article_Date, Article_Audience, Article_Title, Article_Body, Categorys, Contact_Name, Contact_Email, Created_By_Id, Created_Date, Article_Status) VALUES('" + Article.Article_Date + "','" + Article.Article_Audience + "','" + Article.Article_Title + "','" + Article.Article_Body + "','" + Article.Categorys + "','" + Article.Contact_Name + "','" + Article.Contact_Email + "','', GetDate(), 'stored');\n\n";

	return SQL_Query;

}

int main() {

	ifstream inputFile("export.csv");
	ofstream outputFile("queries.txt");
	Article Article;
	bool Extracted = false;

	if (inputFile.is_open())
	{

		std::string Line = "";

		getline(inputFile, Line);

		while (!inputFile.eof())
		{

			Article = Read_Article(inputFile);

			outputFile << Form_Query(Article);

		}

		inputFile.close();

	}
	else std::cout << "Failed to open the file." << std::endl;

	return 0;

}