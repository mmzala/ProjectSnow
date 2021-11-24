#pragma once

class TextWriter
{
public:
	TextWriter(char* font, int fontSize);
	~TextWriter();
	void Write(char* text);
};

