#include <iostream>
#include <string>
#include <sstream>
#include <unistd.h>
#include <vector>
#include <algorithm>
#include "schema.h"
#include "store.h"
using namespace std;

struct InputBuffer
{
    string buffer;
    ssize_t input_length;
};

enum MetaCommandResults
{
    META_COMMAND_SUCCESS,
    META_COMMAND_UNRECOGNIZED_COMMAND
};

enum PrepareResult
{
    PREPARE_SUCCESS,
    PREPARE_UNRECOGNIZED_COMMAND,
    PREPARE_TABLE_FULL,
    PREPARE_INSERT_SYNTAX_ERROR,
    PREPARE_INSERT_ARGUMENT_LENGTH_ERROR,
    PREPARE_OUT_OF_BOUND_ERROR
};

enum StatementType
{
    STATEMENT_INSERT,
    STATEMENT_SELECT,
    STATEMENT_WHERE,
    STATEMENT_HAVING
};

struct Statement
{
    StatementType statement;
    int insert_id;
    string insert_username;
    string insert_email;
};

InputBuffer *new_input_buffer()
{
    InputBuffer *a_new_buffer = new InputBuffer();
    return a_new_buffer;
};

void print_prompt()
{
    cout << "db > ";
    return;
}

void read_input(InputBuffer *reader)
{
    getline(cin, reader->buffer);
    int content_size = reader->buffer.size();
    reader->input_length = content_size;
    return;
}

void close_input_buffer(InputBuffer *reader)
{
    delete reader;
    return;
}

void execute_command(Statement *statement); // Forward declaration

PrepareResult prepare_statement(InputBuffer *reader, Statement *statement, Table* table, int* num_rows)
{
    stringstream ss(reader->buffer);
    string command_type;
    ss >> command_type;
    if(command_type == "insert"){
        statement->statement = STATEMENT_INSERT;
    }
    if(command_type == "select"){
        statement->statement = STATEMENT_SELECT;
    }
    switch (statement->statement)
    {
    case STATEMENT_INSERT:
    {
        if (*num_rows >= MAX_TABLE_ROWS)
        {
            return PREPARE_TABLE_FULL;
        }
        int argument_count = 0;
        vector<string> arguments;
        while (ss >> command_type)
        {
            if (argument_count == 3)
            {
                cout << "Argument count - " << argument_count << endl;
                return PREPARE_INSERT_SYNTAX_ERROR;
            }

            arguments.emplace_back(command_type);
            argument_count++;
        }
        if (argument_count < 3)
        {
            cout << "Argument count - " << argument_count << endl;
            return PREPARE_INSERT_SYNTAX_ERROR;
        }
        Schema schema;
        schema.id = stoi(arguments[0]);
        int username_length = arguments[1].size();
        int email_length = arguments[2].size();
        if (username_length >= 32)
        {
            cout << "Username length - " << username_length << endl;
            return PREPARE_INSERT_ARGUMENT_LENGTH_ERROR;
        }
        if (email_length >= 32)
        {
            cout << "Email length - " << email_length << endl;
            return PREPARE_INSERT_ARGUMENT_LENGTH_ERROR;
        }

        for (int i = 0; i < username_length; i++)
        {
            schema.username[i] = arguments[1][i];
        }

        for (int i = 0; i < email_length; i++)
        {
            schema.email[i] = arguments[2][i];
        }
        return PREPARE_SUCCESS;
    }
    case STATEMENT_SELECT:
        if (*num_rows >= MAX_TABLE_ROWS)
        {
            return PREPARE_OUT_OF_BOUND_ERROR;
        }
        return PREPARE_SUCCESS;
    default : 
        return PREPARE_UNRECOGNIZED_COMMAND;
    }
}

MetaCommandResults execute_meta_commands(InputBuffer *reader)
{
    if (reader->buffer == ".exit")
    {
        close_input_buffer(reader);
        exit(0);
    }
    return META_COMMAND_UNRECOGNIZED_COMMAND;
}
void execute_command(Statement *statement)
{
    switch (statement->statement)
    {
    case STATEMENT_INSERT:
        cout << "This is just a stub for the insert command." << endl;
        break;
    case STATEMENT_SELECT:
        cout << "This is just a stub for the select command." << endl;
        break;

    default:
        cout << "Unrecognized command" << endl;
        break;
    }
}

int main()
{
    InputBuffer *input_buffer = new_input_buffer();
    int num_rows = 0;
    Table table;
      table.num_rows = MAX_TABLE_ROWS;
    while (true)
    {
      
        print_prompt();
        read_input(input_buffer);

        if (input_buffer->buffer.empty())
            continue;

        if (input_buffer->buffer[0] == '.')
        {
            switch (execute_meta_commands(input_buffer))
            {
            case META_COMMAND_SUCCESS:
                continue;
            case META_COMMAND_UNRECOGNIZED_COMMAND:
                cout << "Unrecognized command '" << input_buffer->buffer << "'" << endl;
                continue;
            }
        }

        Statement statement;
        switch (prepare_statement(input_buffer, &statement, &table, &num_rows))
        {
        case PREPARE_SUCCESS:
            break;
        case PREPARE_UNRECOGNIZED_COMMAND:
            cout << "Unrecognized keyword at start of '" << input_buffer->buffer << "'." << endl;
            break;
        case PREPARE_INSERT_SYNTAX_ERROR:
            cout << "Expected 3 arguments." << endl;
            break;
        case PREPARE_INSERT_ARGUMENT_LENGTH_ERROR:
            cout << "Expected length is 32." << endl;
            break;
        case PREPARE_TABLE_FULL:
            cout << "The table is full!" << endl;
            break;
        case PREPARE_OUT_OF_BOUND_ERROR:
            cout << "Requested Data is out of bounds for the table." << endl;
            break;
        }

        execute_command(&statement);
        cout << "Executed." << endl;
    }

    return 0;
}