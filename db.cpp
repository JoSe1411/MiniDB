#include<iostream>
#include<string>
#include<sstream>
#include<vector>
#include<algorithm>
#include "schema.h"
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
    PREPARE_UNRECOGNIZED_COMMAND,
    PREPARE_INSERT_SYNTAX_ERROR,
    PREPARE_INSERT_ARGUMENT_LENGTH_ERROR
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
        // now we extract the relevant information for our insert command
        // format - insert 1    joy_sen       example@gmail.com
        //                (id)  (username)        (email)

        int argument_count = 0;
        vector<string> arguments;
        while(ss>>command_type){
             if(argument_count == 3){
                cout<<"Argument count - "<<argument_count<<endl;
                return PREPARE_INSERT_SYNTAX_ERROR;
             }
            
            arguments.emplace_back(command_type);
            argument_count++;
        }
        if(argument_count<3){
            cout<<"Argument count - "<<argument_count<<endl;
            return PREPARE_INSERT_SYNTAX_ERROR;
        }
        int user_id = stoi(arguments[0]);
        char username[32];
        char email[32];
        int username_length = arguments[1].size();
        int email_length = arguments[2].size();
        if(username_length>=32){
            cout<<"Username length - "<<username_length<<endl;
            return PREPARE_INSERT_ARGUMENT_LENGTH_ERROR;
        }
        if(email_length>=32){
            cout<<"Email length - "<<email_length<<endl;
            return PREPARE_INSERT_ARGUMENT_LENGTH_ERROR;
        }

        for(int i =0 ; i<username_length;i++){
            username[i] = arguments[1][i];
        }

        for(int i =0 ; i<email_length ; i++){
            email[i] = arguments[2][i];
        }

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
                break;
            case PREPARE_INSERT_SYNTAX_ERROR:
                cout<<"Expected 3 arguments."<<endl;
                break;
            case PREPARE_INSERT_ARGUMENT_LENGTH_ERROR:
                cout<<"Expected length is 32."<<endl;
                continue;
        }

        execute_command(&statement);
        cout << "Executed." << endl;
    }

    return 0;
}

    