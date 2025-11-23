#include<iostream>
#include<string>
#include<sstream>
#include<vector>
#include<algorithm>
using namespace std;

struct InputBuffer{
    string buffer;
    ssize_t input_length;
};

enum MetaCommandResults{
    META_COMMAND_SUCCESS,
    META_COMMAND_UNRECOGNIZED_COMMAND
};

enum PrepareResult{
    PREPARE_SUCCESS,
    PREPARE_UNRECOGNIZED_COMMAND
};

enum StatementType{
    STATEMENT_INSERT,
    STATEMENT_SELECT,
    STATEMENT_WHERE,
    STATEMENT_HAVING
};

struct Statement{
    StatementType statement;
};

InputBuffer* new_input_buffer(){
    InputBuffer* a_new_buffer = new InputBuffer();
    return a_new_buffer;
};

void print_prompt(){
    cout<<"db > ";
    return;
}


void read_input(InputBuffer* reader){
    getline(cin,reader->buffer);
    int content_size = reader->buffer.size();
    reader->input_length = content_size;
    return;
}

void close_input_buffer(InputBuffer* reader){
     delete reader;
     return;
}


void execute_command(Statement* statement); // Forward declaration

PrepareResult prepare_statement(InputBuffer* reader,Statement* statement){    
    stringstream ss(reader->buffer);
    string command_type;
    ss >> command_type;

    if(command_type == "insert"){
        statement->statement = STATEMENT_INSERT;
        return PREPARE_SUCCESS;
    }
    if(command_type == "select"){
        statement->statement = STATEMENT_SELECT;
        return PREPARE_SUCCESS;
    }
    return PREPARE_UNRECOGNIZED_COMMAND;
}


MetaCommandResults execute_meta_commands(InputBuffer* reader){
    if(reader->buffer == ".exit"){ 
        close_input_buffer(reader);
        exit(0);
    }
    return META_COMMAND_UNRECOGNIZED_COMMAND;
}

void execute_command(Statement* statement){
        switch(statement->statement){
            case STATEMENT_INSERT:
                cout<<"This is just a stub for the insert command."<<endl;
                break;

            case STATEMENT_SELECT:
                cout<<"This is just a stub for the select command."<<endl;
                break;
            
            default:
                cout<<"Unrecognized command"<<endl;
                break;
        }
}


int main (){
    InputBuffer* input_buffer = new_input_buffer();
    while(true){
        print_prompt();
        read_input(input_buffer);

        if (input_buffer->buffer.empty()) continue;

        if (input_buffer->buffer[0] == '.') {
            switch (execute_meta_commands(input_buffer)) {
                case META_COMMAND_SUCCESS:
                    continue;
                case META_COMMAND_UNRECOGNIZED_COMMAND:
                    cout << "Unrecognized command '" << input_buffer->buffer << "'" << endl;
                    continue;
            }
        }

        Statement statement;
        switch (prepare_statement(input_buffer, &statement)) {
            case PREPARE_SUCCESS:
                break;
            case PREPARE_UNRECOGNIZED_COMMAND:
                cout << "Unrecognized keyword at start of '" << input_buffer->buffer << "'." << endl;
                continue;
        }

        execute_command(&statement);
        cout << "Executed." << endl;
    }

    return 0;
}

    