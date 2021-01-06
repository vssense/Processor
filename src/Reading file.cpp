
struct str
{
    char* str = nullptr;
    int   len =       0;
};

struct text
{
    size_t num_symbols = 0;
    size_t num_str = 0;
    str* lines = nullptr;
    char* start = nullptr;
};


//-----------------------------------------------
//!  calculates file length
//!
//!  @param [in]   in  file
//!
//!  @return lenght of file
//-----------------------------------------------

size_t LenOfFile(FILE* in)
{
    assert(in != nullptr);

    fseek(in, 0, SEEK_END);
    size_t pos = ftell(in);
    fseek(in, 0, SEEK_SET);

    return pos;
}

//-----------------------------------------------
//!  reading from file to buffer
//!
//!  @param [in] in           file read from
//!  @param [in] num_symbols  number of symbols in the text
//!  @param [in] start        pointer on start of text
//!
//!  @note if you have windows, function will clear text from '\r'
//-----------------------------------------------

size_t ReadFile(FILE* in, size_t num_symbols, char* start)
{
    assert(in);
    assert(start);

    size_t real_num_symbols = fread(start, sizeof(char), num_symbols, in);
    
    #if _WIN32
        start[real_num_symbols + 2] = '\0';
    #endif
    return real_num_symbols;
}

//-----------------------------------------------
//!  calculating number of lines
//!
//!  @param [in] start        pointer on start of text
//!  @param [in] num_symbols  number of symbols in the text
//!
//!  @return number of lines
//-----------------------------------------------

int NumLines(const char* start, size_t num_symbols)
{
    assert(start);

    int num = 1;
    for (size_t i = 0; i < num_symbols; ++i)
        if (start[i] == '\n') num++;
    return num;
}

//-----------------------------------------------
//!  making lines from the text
//!
//!  @param [in] lines        pointer to struct where kept string and lenght
//!  @param [in] num_symbols  number of symbols in the text
//!  @param [in] start        pointer on start of text
//!
//!  @note also calculates lenght of each string
//-----------------------------------------------

void MakeLines(char* start, str* lines, size_t num_symbols)
{
    assert(start);
    assert(lines);

    int lastI    = 0;
    int Num_line = 0;

    lines[Num_line++].str = start;

    for (size_t i = 0; i < num_symbols; ++i)
    {
        if (start[i] == '\n')
        {
            start[i] = '\0';
            lines[Num_line - 1].len = i - lastI;
            lastI = i + 1;
            lines[Num_line++].str = &start[i+1];
        }
    }
    lines[Num_line - 1].len = num_symbols - lastI;
}

//-----------------------------------------------
//!  printing lines
//!
//!  @param [in] lines  pointer to struct where kept string and lenght
//!  @param [in] num    number of strings in the text
//!  @param [in] out    file where we printing
//!
//-----------------------------------------------

void PrintLines(str* lines, size_t num, FILE* const out)
{
    assert(lines);
    assert(out);

    for (size_t j = 0; j < num; j++)
    {
        fprintf(out, "%s\n", lines[j].str);
    }
}

//------------------------------------------------
//!  reading file, making lines, calculating number of
//!  lines and symbols
//!
//!  @param [in] text  pointer to struct text
//!  @param [in] in      file read from
//!
//!  @note text includes num_symbols, num_str,
//!             pointer start and pointer lines
//-----------------------------------------------

void ReadTextAndMakeLines(text* text, FILE* in)
{
    assert(text);

    text->num_symbols = LenOfFile(in);
    text->start       = (char*)calloc(text->num_symbols + 1, sizeof(char));
    text->num_symbols = ReadFile(in, text->num_symbols, text->start);
    text->num_str     = NumLines(text->start, text->num_symbols);
    text->lines       = (str*)calloc(text->num_str + 1, sizeof(str));
    MakeLines(text->start, text->lines, text->num_symbols);
}

void DestructText(text* text)
{
    assert(text);
    assert(text->start);
    assert(text->lines);

    free(text->start);
    free(text->lines);

    text->start = nullptr;
    text->lines = nullptr;
    text->num_str = 0;
    text->num_symbols = 0;
}
