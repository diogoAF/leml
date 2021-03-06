#define CLASSMANAGER_SERV
#include <stdio.h>
#include <string.h>
#include <math.h>
#include <stdint.h>

#include "lib/UserViewer.h"

///Funcoes de Debug (Serao apagadas depois de cumprirem sua funcao)
void printFrame(Frame *frame, FILE *output);
void printHandler(ClassHandler *handler, FILE *output);
void printFields(u2 fields_count, field_info *fields, FILE *output);
void printLocalVariableArray(u2 attributes_count, LocalVariable *localVariableArray, FILE *output);
void printOperandStack(structOperandStack ** operandStack, FILE *output);


void classPrint(ClassFile * class_file){
    char string[100];

    FILE *output;
    output = fopen("output.txt", "w");

	fprintf( output, "Magic Number:\t\t0x%08x\n",class_file->magic);
    fprintf( output, "Minor Version:\t\t%d\n",class_file->minor_version);
    if(class_file->major_version > 48) //Condi��o para imprimir o n�mero da vers�o comercial do Java
        fprintf( output, "Major Version:\t\t%d [%d]\n",class_file->major_version, class_file->major_version%44);
    else
        fprintf( output, "Major Version:\t\t%d [1.%d]\n",class_file->major_version, class_file->major_version%44);

    fprintf( output, "Constant Pool Count:\t%d\n",class_file->constant_pool_count);
    string[0] = '\0';
    getAccessFlag(class_file->access_flags, &string, 0, output);
    fprintf( output, "Access flags:\t\t0x%04x [%s]\n",class_file->access_flags, string);
    string[0] = '\0';
    selectPointer(class_file, class_file->this_class, &string, 0);
    fprintf( output, "This class:\t\tcp_info #%d <%s>\n",class_file->this_class, string);
    string[0] = '\0';
    selectPointer(class_file, class_file->super_class, &string, 0);
    fprintf( output, "Super class:\t\tcp_info #%d <%s>\n",class_file->super_class, string);
    fprintf( output, "Interfaces Count:\t%d\n",class_file->interfaces_count);
    fprintf( output, "Fields Count:\t\t%d\n",class_file->fields_count);
    fprintf( output, "Methods Count:\t\t%d\n",class_file->methods_count);
    fprintf( output, "Attributes Count:\t%d\n",class_file->attributes_count);

	fprintf( output, "\n+Constant Pool:\n");
	printConstantPool(class_file->constant_pool,class_file->constant_pool_count, class_file, output);

    fprintf( output, "\n+Interfaces\n");
	for(int i = 0; i < class_file->interfaces_count; i++){
        string[0] = '\0';
        selectPointer(class_file, class_file->interfaces[i], &string, 0);
		fprintf( output, "\tInterface %d:\t\tcp_info #%d <%s>\n", i,class_file->interfaces[i], string);
	}
	fprintf( output, "\n+Fields\n");
    fprintf( output, "\tMember Count:\t\t%d\n",class_file->fields_count);
	for(int i = 0; i < class_file->fields_count; i++){
        string[0] = '\0';
        selectPointer(class_file, class_file->fields[i].name_index, &string, 0);
        fprintf( output, "\n\t[%d] %s", i, string);
		fprintf( output, "\tName:\t\t\t\tcp_info #%d <%s>\n",class_file->fields[i].name_index, string);
		string[0] = '\0';
        selectPointer(class_file, class_file->fields[i].descriptor_index, &string, 0);
        fprintf( output, "\t\t\tDescriptor:\t\t\tcp_info #%d <%s>\n",class_file->fields[i].descriptor_index, string);
        string[0] = '\0';
        getAccessFlag(class_file->fields[i].access_flags, &string, 0, output);
        fprintf( output, "\t\t\tAccess flags:\t\t\t0x%04x [%s]\n",class_file->fields[i].access_flags, string);

        printAttributesInfo(class_file->fields[i].attributes, class_file->fields[i].attributes_count, class_file->constant_pool, class_file, output);
 	}

    fprintf( output, "\n+Methods\n");
    fprintf( output, "\tMember Count:\t\t%d\n",class_file->methods_count);
	///Chama a fun��o responsavl por printar os methods
	printMethodInfo(class_file->methods, class_file->methods_count, class_file, output);

    fprintf( output, "\n+Attributes\n");
	printAttributesInfo(class_file->attributes,class_file->attributes_count, class_file->constant_pool, class_file, output);
    fclose(output);
}

void printConstantPool(cp_info * constant_pool, u2 constant_pool_count, ClassFile *class_file, FILE* output){
    char string[100];
    u4 var_u4;
    float var_float;
    int64_t var_64;
    double var_double;
    for(int i = 1; i < constant_pool_count; i++){
		 switch(constant_pool[i].tag){
			case CONSTANT_Class:
			    string[0] = '\0';
			    selectPointer(class_file, i, &string, 0);
				fprintf( output, "\n[%02d]CONSTANT_Class:", i);
                fprintf( output, "\t\tClass Name:\tcp_info #%d <%s>\n",constant_pool[i].Class.name_index, string);
				break;
			case CONSTANT_Fieldref:
			    string[0] = '\0';
			    selectPointer(class_file, i, &string, 0);
				fprintf( output, "\n[%02d]CONSTANT_Fieldref:", i);
				fprintf( output, "\t\tClass name:\tcp_info #%d <%s>\n",constant_pool[i].Fieldref.class_index, string);
				string[0] = '\0';
			    selectPointer(class_file, i, &string, 1);
				fprintf( output, "\t\t\t\tName and type:\tcp_info #%d <%s>\n",constant_pool[i].Fieldref.name_and_type_index, string);
				break;
			case CONSTANT_Methodref:
			    string[0] = '\0';
			    selectPointer(class_file, i, &string, 0);
				fprintf( output, "\n[%02d]CONSTANT_Methodref:", i);
				fprintf( output, "\t\tClass name:\tcp_info #%d <%s>\n",constant_pool[i].Methodref.class_index, string);
				string[0] = '\0';
			    selectPointer(class_file, i, &string, 1);
				fprintf( output, "\t\t\t\tName and type:\tcp_info #%d <%s>\n",constant_pool[i].Methodref.name_and_type_index, string);
				break;
			case CONSTANT_InterfaceMethodref:
				fprintf( output, "\n[%02d]CONSTANT_InterfaceMethodref:",i);
				string[0] = '\0';
			    selectPointer(class_file, i, &string, 0);
				fprintf( output, "\t\tClass name:\tcp_info #%d <%s>\n",constant_pool[i].InterfaceMethodref.class_index, string);
				string[0] = '\0';
			    selectPointer(class_file, i, &string, 1);
				fprintf( output, "\t\t\t\tName and type:\tcp_info #%d <%s>\n",constant_pool[i].InterfaceMethodref.name_and_type_index, string);
				break;
			case CONSTANT_String:
				fprintf( output, "\n[%02d]CONSTANT_String",i);
				string[0] = '\0';
			    selectPointer(class_file, i, &string, 0);
                fprintf( output, "\t\tString:\t\tcp_info #%d <%s>\n",constant_pool[i].String.string_index, string);
				break;
			case CONSTANT_Integer:
				fprintf( output, "\n[%02d]CONSTANT_Integer", i);
                fprintf( output, "\t\tBytes:\t\t\t0x%04x\n",constant_pool[i].Integer.bytes);
                fprintf( output, "\t\t\t\tInteger:\t\t%d\n",constant_pool[i].Integer.bytes);
				break;
			case CONSTANT_Float:
				fprintf( output, "\n[%02d]CONSTANT_Float", i);
				fprintf( output, "\t\tBytes:\t0x%04x\n",constant_pool[i].Float.bytes);
				var_u4 = constant_pool[i].Float.bytes;
				var_float = *((float*)&var_u4);
				fprintf( output, "\t\t\t\tFloat:\t%.4f\n",var_float);
				break;
			case CONSTANT_Long:
				fprintf( output, "\n[%02d]CONSTANT_Long",i);
				fprintf( output, "\t\tHIGH_BYTES:\t\t0x%08x\n",constant_pool[i].Long.high_bytes);
				fprintf( output, "\t\t\t\tLOW_BYTES:\t\t0x%04x\n",constant_pool[i].Long.low_bytes);
				var_64 = constant_pool[i].Long.high_bytes;
				var_64 = (var_64 << 32) | constant_pool[i].Long.low_bytes;
				fprintf( output, "\t\t\t\tLong:\t\t\t%ld\n", var_64);
                i++;
                fprintf( output, "\n[%02d](large numeric continued)\n", i);
				break;
			case CONSTANT_Double:
				fprintf( output, "\n[%02d]CONSTANT_Double",i);
				fprintf( output, "\t\tHigh bytes:\t\t0x%08x\n",constant_pool[i].Double.high_bytes);
				fprintf( output, "\t\t\t\tLow bytes:\t\t0x%08x\n",constant_pool[i].Double.low_bytes);
                var_64 = constant_pool[i].Double.high_bytes;
                var_64 = (var_64 << 32) | constant_pool[i].Double.low_bytes;
                var_double = *((double*)&var_64);
                fprintf( output, "\t\t\t\tDouble:\t\t\t%f\n", var_double);
				i++;
				fprintf( output, "\n[%02d](large numeric continued)\n", i);
				break;
			case CONSTANT_NameAndType:
				fprintf( output, "\n[%02d]CONSTANT_NameAndType", i);
				string[0] = '\0';
			    selectPointer(class_file, i, &string, 0);
				fprintf( output, "\tName:\t\tcp_info #%d <%s>\n",constant_pool[i].NameAndType.name_index, string);
				string[0] = '\0';
			    selectPointer(class_file, i, &string, 1);
				fprintf( output, "\t\t\t\tDescriptor:\tcp_info #%d <%s>\n",constant_pool[i].NameAndType.descriptor_index, string);
				break;
			case CONSTANT_Utf8:
				fprintf( output, "\n[%02d]CONSTANT_Utf8",i);
				fprintf( output, "\t\tLength of byte array:\t%d\n",constant_pool[i].UTF8.length);
				fprintf( output, "\t\t\t\tLength of string:\t%d\n",constant_pool[i].UTF8.length);
				fprintf( output, "\t\t\t\tString:\t\t\t%s\n",constant_pool[i].UTF8.bytes);
				break;
			default:
                break;
		 }
	}
}

void printMethodInfo(method_info * methods, u2 methods_count, ClassFile *class_file, FILE* output){
    char string[100];
    for(int i = 0; i < methods_count; i++){
        string[0] = '\0';
        selectPointer(class_file, methods[i].name_index, &string, 0);
        fprintf( output, "\n\t[%d] %s\n", i, string);
		fprintf( output, "\t\tName:\t\t\tcp_info #%d <%s>\n",methods[i].name_index, string);
		string[0] = '\0';
        selectPointer(class_file, methods[i].descriptor_index, &string, 0);
		fprintf( output, "\t\tDescriptor:\t\tcp_info #%d <%s>\n",methods[i].descriptor_index, string);
		string[0] = '\0';
        getAccessFlag(methods[i].access_flags, &string, 1, output);
		fprintf( output, "\t\tAccess flags:\t\t0x%04x [%s]\n",methods[i].access_flags, string);

		printAttributesInfo(methods[i].attributes,methods[i].attributes_count, class_file->constant_pool, class_file, output);
	}
}

void printAttributesInfo(attribute_info * attributes, u2 attributes_count, cp_info * constant_pool, ClassFile *class_file, FILE *output){
	char *indice;
	char string[100];

	for(int j = 0; j < attributes_count; j++){
		indice = constant_pool[attributes[j].name_index].UTF8.bytes;

		fprintf( output, "\n\t\t[%d] %s\n", j, indice);
        fprintf( output, "\t\t\tAttribute name index:\t\tcp_info #%d\n",attributes[j].name_index);
        fprintf( output, "\t\t\tAttribute length:\t\t%d\n",attributes[j].length);

		switch(attributes[j].tag){
			case ATTRIBUTE_ConstantValue:
                string[0] = '\0';
                selectPointer(class_file, attributes[j].ConstantValue.index, &string, 0);
				fprintf( output, "\t\t\tConstant value index:\t\tcp_info #%d <%s>\n",attributes[j].ConstantValue.index, string);
				break;
			case ATTRIBUTE_Code:
			    fprintf( output, "\t\tBytecode:\n");
			    fprintf( output, "\t\t-------------------------------------------\n");
				printCode(attributes[j].Code.code, attributes[j].Code.code_length, constant_pool, output);
				fprintf( output, "\t\t-------------------------------------------\n");
				fprintf( output, "\n\t\tException table:\n");
				fprintf( output, "\t----------------------------------------------------------------------\n");
				fprintf( output, "\tNr.  |  start_pc  |  end_pc  |  handler_pc  |  catch_type  |  verbose\n");
				for(int k =0; k < attributes[j].Code.exception_table_length; k++){
                    fprintf( output, "\t\t%d\t|\t%d\t|\t%d\t|\t%d\t|\t%d\n", k, attributes[j].Code.ExceptionTable[k].start_pc, \
                           attributes[j].Code.ExceptionTable[k].end_pc, attributes[j].Code.ExceptionTable[k].handler_pc, \
                           attributes[j].Code.ExceptionTable[k].catch_type);
                }
                fprintf( output, "\t----------------------------------------------------------------------\n");
				fprintf( output, "\n\t\tMisc:\n");
				fprintf( output, "\t\tMaximum stack depth:\t\t%d\n",attributes[j].Code.max_stack);
				fprintf( output, "\t\tMaximum local variables:\t%d\n",attributes[j].Code.max_locals);
				fprintf( output, "\t\tCode length:\t\t\t%d\n",attributes[j].Code.code_length);

				printAttributesInfo(attributes[j].Code.attributes, attributes[j].Code.attributes_count,class_file->constant_pool, class_file, output);

				break;
			case ATTRIBUTE_Exceptions:
				fprintf( output, "\t\tExceptions.number_of_exceptions: %d\n",attributes[j].Exceptions.number_of_exceptions);
				for(int k =0; k < attributes[j].Exceptions.number_of_exceptions; k++){
					fprintf( output, "\t\tExceptions.exception_index_table[%d]: %d\n",k,attributes[j].Exceptions.exception_index_table[k]);
				}
				break;
			case ATTRIBUTE_InnerClasses:
				fprintf( output, "\t\tInnerClasses.number_of_classes: %d\n",attributes[j].InnerClasses.number_of_classes);
				break;
			case ATTRIBUTE_Syntethic:
				break;
			case ATTRIBUTE_LineNumber:
				fprintf( output, "\t\t\tNr.\t    start_pc\t    line_number\n");
				fprintf( output, "\t\t-------------------------------------------------\n");
				for(int i=0; i<attributes[j].LineNumber.line_number_table_length; i++){
                    fprintf( output, "\t\t\t%d\t\t%d\t\t%d\n", i, attributes[j].LineNumber.Line_number_table[i].start_pc, attributes[j].LineNumber.Line_number_table[i].line_number);
				}
				fprintf( output, "\t\t-------------------------------------------------\n");
                break;
			case ATTRIBUTE_LocalVariable:
				fprintf( output, "\t\tLocalVariable.local_variable_table_length: %d\n",attributes[j].LocalVariable.local_variable_table_length);
				break;
			case ATTRIBUTE_SourceFile:
			    string[0] = '\0';
                selectPointer(class_file, attributes[j].SourceFile.sourcefile_index, &string, 0);
				fprintf( output, "\t\t\tSource file name index:\t\tcp_info #%d <%s>\n",attributes[j].SourceFile.sourcefile_index, string);
                break;
			default: //ATTRIBUTE_Unknown
                fprintf( output, "\t\tATRIBUTO DESCONHECIDO:\n");
                break;
		}

	}

}


void getAccessFlag(int flag, char *string, int tipo, FILE* output){
    switch (tipo){
    case (0):
        if(flag == 0){
            return;
        }else if(flag%ACC_STRICT == 0){
            flag -= ACC_STRICT;
            getAccessFlag(flag, string, tipo, output);
            strcat(string, "strict ");
        }else if(flag%ACC_ABSTRACT == 0){
            flag -= ACC_ABSTRACT;
            getAccessFlag(flag, string, tipo, output);
            strcat(string, "abstract ");
        }else if(flag%ACC_INTERFACE == 0){
            fprintf( output, "inter\n");
            flag -= ACC_INTERFACE;
            getAccessFlag(flag, string, tipo, output);
            strcat(string, "interface ");
        }else if(flag%ACC_NATIVE == 0){
            flag -= ACC_NATIVE;
            getAccessFlag(flag, string, tipo, output);
            strcat(string, "native ");
        }else if(flag%ACC_TRANSIENT == 0){
            flag -= ACC_TRANSIENT;
            getAccessFlag(flag, string, tipo, output);
            strcat(string, "transient ");
        }else if(flag%ACC_VOLATILE == 0){
            flag -= ACC_VOLATILE;
            getAccessFlag(flag, string, tipo, output);
            strcat(string, "volatile ");
        }else if(flag%33 == 0){ //considera o 0x0021 como public
            flag -= 33;
            getAccessFlag(flag, string, tipo, output);
            strcat(string, "public ");
        }else if(flag%ACC_SUPER == 0){
            flag -= ACC_SUPER;
            getAccessFlag(flag, string, tipo, output);
            strcat(string, "super ");
        }else if(flag%ACC_FINAL == 0){
            flag -= ACC_FINAL;
            getAccessFlag(flag, string, tipo, output);
            strcat(string, "final ");
        }else if(flag%ACC_STATIC == 0){
            flag -= ACC_STATIC;
            getAccessFlag(flag, string, tipo, output);
            strcat(string, "static ");
        }else if(flag%ACC_PROTECTED == 0){
            flag -= ACC_PROTECTED;
            getAccessFlag(flag, string, tipo, output);
            strcat(string, "protect ");
        }else if(flag%ACC_PRIVATE == 0){
            flag -= ACC_PRIVATE;
            getAccessFlag(flag, string, tipo, output);
            strcat(string, "private ");
        }else if(flag%ACC_PUBLIC == 0){
            flag -= ACC_PUBLIC;
            getAccessFlag(flag, string, tipo, output);
            strcat(string, "public ");
        }
        break;
    case (1):
        if(flag == 0){
            return;
        }else if(flag%ACC_STRICT == 0){
            flag -= ACC_STRICT;
            getAccessFlag(flag, string, tipo, output);
            strcat(string, "strict ");
        }else if(flag%ACC_ABSTRACT == 0){
            flag -= ACC_ABSTRACT;
            getAccessFlag(flag, string, tipo, output);
            strcat(string, "abstract ");
        }else if(flag%ACC_INTERFACE == 0){
            fprintf( output, "inter\n");
            flag -= ACC_INTERFACE;
            getAccessFlag(flag, string, tipo, output);
            strcat(string, "interface ");
        }else if(flag%ACC_NATIVE == 0){
            flag -= ACC_NATIVE;
            getAccessFlag(flag, string, tipo, output);
            strcat(string, "native ");
        }else if(flag%ACC_TRANSIENT == 0){
            flag -= ACC_TRANSIENT;
            getAccessFlag(flag, string, tipo, output);
            strcat(string, "transient ");
        }else if(flag%ACC_VOLATILE == 0){
            flag -= ACC_VOLATILE;
            getAccessFlag(flag, string, tipo, output);
            strcat(string, "volatile ");
        }else if(flag%33 == 0){ //considera o 0x0021 como public
            flag -= 33;
            getAccessFlag(flag, string, tipo, output);
            strcat(string, "public ");
        }else if(flag%ACC_SUPER == 0){
            flag -= ACC_SYNCHRONIZED;
            getAccessFlag(flag, string, tipo, output);
            strcat(string, "syncronized ");
        }else if(flag%ACC_FINAL == 0){
            flag -= ACC_FINAL;
            getAccessFlag(flag, string, tipo, output);
            strcat(string, "final ");
        }else if(flag%ACC_STATIC == 0){
            flag -= ACC_STATIC;
            getAccessFlag(flag, string, tipo, output);
            strcat(string, "static ");
        }else if(flag%ACC_PROTECTED == 0){
            flag -= ACC_PROTECTED;
            getAccessFlag(flag, string, tipo, output);
            strcat(string, "protect ");
        }else if(flag%ACC_PRIVATE == 0){
            flag -= ACC_PRIVATE;
            getAccessFlag(flag, string, tipo, output);
            strcat(string, "private ");
        }else if(flag%ACC_PUBLIC == 0){
            flag -= ACC_PUBLIC;
            getAccessFlag(flag, string, tipo, output);
            strcat(string, "public ");
        }
        break;
    //case ('a'):
      //  break;
    }
}

//Fun��o respons�vel por selecionar a op��o dentro do CONSTANT_x quando esta possui mais de dois indices
void selectPointer(ClassFile * class_file, int indice, char *string, int option){

    switch (class_file->constant_pool[indice].tag){
    case (CONSTANT_Class):
        catchString(class_file, class_file->constant_pool[indice].Class.name_index, string);
        break;
    case (CONSTANT_String):
        catchString(class_file, class_file->constant_pool[indice].String.string_index, string);
        break;
    case (CONSTANT_Methodref):
        if(option == 0)
            catchString(class_file, class_file->constant_pool[indice].Methodref.class_index, string);
        else
            catchString(class_file, class_file->constant_pool[indice].Methodref.name_and_type_index, string);
        break;
    case (CONSTANT_Fieldref):
        if(option == 0)
            catchString(class_file, class_file->constant_pool[indice].Fieldref.class_index, string);
        else
            catchString(class_file, class_file->constant_pool[indice].Fieldref.name_and_type_index, string);
        break;
    case(CONSTANT_NameAndType):
        if(option == 0)
            catchString(class_file, class_file->constant_pool[indice].NameAndType.name_index, string);
        else
            catchString(class_file, class_file->constant_pool[indice].NameAndType.descriptor_index, string);
        break;
    case (CONSTANT_InterfaceMethodref):
        if(option == 0)
            catchString(class_file, class_file->constant_pool[indice].InterfaceMethodref.class_index, string);
        else
            catchString(class_file, class_file->constant_pool[indice].InterfaceMethodref.class_index, string);
        break;
    default:
        catchString(class_file, indice, string);
        break;
    }
}


//Fun��o para montar a string caso ela possua referencia para um ponteiro que n�o seja um Utf8 \
Lembrando que uma vez que tenham dois ponteiros dentro de uma CONSTANT_x, as strings devem ser concatenadas
void catchString(ClassFile * class_file, int indice, char *string){

    u1 *s1;
    s1 = (u1*)malloc(100*sizeof(u1));
    int64_t var_64;
    float var_float;
    double var_double;

    switch (class_file->constant_pool[indice].tag){
    case (CONSTANT_Class):
        catchString(class_file, class_file->constant_pool[indice].Class.name_index, string);
        break;
    case (CONSTANT_String):
        catchString(class_file, class_file->constant_pool[indice].String.string_index, string);
        break;
    case (CONSTANT_Methodref):
        catchString(class_file, class_file->constant_pool[indice].Methodref.class_index, string);
        catchString(class_file, class_file->constant_pool[indice].Methodref.name_and_type_index, string);
        break;
    case (CONSTANT_Fieldref):
        catchString(class_file, class_file->constant_pool[indice].Fieldref.class_index, string);
        catchString(class_file, class_file->constant_pool[indice].Fieldref.name_and_type_index, string);
        break;
    case(CONSTANT_NameAndType):
        catchString(class_file, class_file->constant_pool[indice].NameAndType.name_index, string);
        catchString(class_file, class_file->constant_pool[indice].NameAndType.descriptor_index, string);
        break;
    case (CONSTANT_InterfaceMethodref):
        catchString(class_file, class_file->constant_pool[indice].InterfaceMethodref.class_index, string);
        catchString(class_file, class_file->constant_pool[indice].InterfaceMethodref.class_index, string);
        break;
    case (CONSTANT_Utf8):
        s1 = class_file->constant_pool[indice].UTF8.bytes;
        strcat(string, s1);
        break;
    case (CONSTANT_Integer):
        sprintf(s1, "%d", class_file->constant_pool[indice].Integer.bytes);
        strcat(string, s1);
        break;
    case (CONSTANT_Float):
        var_float = *((float*)&(class_file->constant_pool[indice].Float.bytes));
        sprintf(s1, "%f", var_float);
        strcat(string, s1);
        break;
    case (CONSTANT_Long):
        var_64 = class_file->constant_pool[indice].Long.high_bytes;
        var_64 = (var_64 << 32) | class_file->constant_pool[indice].Long.low_bytes;
        sprintf(s1, "%ld", var_64);
        strcat(string, s1);
        break;
    case (CONSTANT_Double):
        var_64 = class_file->constant_pool[indice].Double.high_bytes;
        var_64 = (var_64 << 32) | class_file->constant_pool[indice].Double.low_bytes;
        var_double = *((double*)&var_64);
        sprintf(s1, "%f", var_double);
        strcat(string, s1);
        break;
    }
}

void printCode(u1 *code, u4 code_length, cp_info * constant_pool, FILE *output){
    u2 var_u2;
    u2 index1, index2;
    for(int k = 0; k < code_length; k++){
    fprintf( output, "\t\t\t%2d  ",k);
        switch(code[k]){
            case OPCODE_nop:
                fprintf( output, "nop\n");
                break;
            case OPCODE_aconst_null:
                fprintf( output, "aconst_null\n");
                break;
            case OPCODE_iconst_m1:
                fprintf( output, "iconst_m1\n");
                break;
            case OPCODE_iconst_0:
                fprintf( output, "iconst_0\n");
                break;
            case OPCODE_iconst_1:
                fprintf( output, "iconst_1\n");
                break;
            case OPCODE_iconst_2:
                fprintf( output, "iconst_2\n");
                break;
            case OPCODE_iconst_3:
                fprintf( output, "iconst_3\n");
                break;
            case OPCODE_iconst_4:
                fprintf( output, "iconst_4\n");
                break;
            case OPCODE_iconst_5:
                fprintf( output, "iconst_5\n");
                break;
            case OPCODE_lconst_0:
                fprintf( output, "lconst_0\n");
                break;
            case OPCODE_lconst_1:
                fprintf( output, "lconst_1\n");
                break;
            case OPCODE_fconst_0:
                fprintf( output, "fconst_0\n");
                break;
            case OPCODE_fconst_1:
                fprintf( output, "fconst_1\n");
                break;
            case OPCODE_fconst_2:
                fprintf( output, "fconst_2\n");
                break;
            case OPCODE_dconst_0:
                fprintf( output, "dconst_0\n");
                break;
            case OPCODE_dconst_1:
                fprintf( output, "dconst_1\n");
                break;
            case OPCODE_bipush:
                fprintf( output, "bipush %d\n", code[k+1]);
                k++;
                break;
            case OPCODE_sipush:
                fprintf( output, "sipush %d %d\n", code[k+1], code[k+2]);
                k=k+2;
                break;
            case OPCODE_ldc:
                fprintf( output, "ldc %d\n", code[k+1]);
                k++;
                break;
            case OPCODE_ldc_w:
                fprintf( output, "ldc_w %d %d\n", code[k+1], code[k+2]);
                k=k+2;
                break;
            case OPCODE_ldc2_w:
                fprintf( output, "ldc2_w %d %d\n", code[k+1], code[k+2]);
                k=k+2;
                break;
            case OPCODE_iload:
                fprintf( output, "iload %d\n", code[k+1]);
                k++;
                break;
            case OPCODE_lload:
                fprintf( output, "lload %d\n", code[k+1]);
                k++;
                break;
            case OPCODE_fload:
                fprintf( output, "fload %d\n", code[k+1]);
                k++;
                break;
            case OPCODE_dload:
                fprintf( output, "dload %d\n", code[k+1]);
                k++;
                break;
            case OPCODE_aload:
                fprintf( output, "aload %d\n", code[k+1]);
                k++;
                break;
            case OPCODE_iload_0:
                fprintf( output, "iload_0\n");
                break;
            case OPCODE_iload_1:
                fprintf( output, "iload_1\n");
                break;
            case OPCODE_iload_2:
                fprintf( output, "iload_2\n");
                break;
            case OPCODE_iload_3:
                fprintf( output, "iload_3\n");
                break;
            case OPCODE_lload_0:
                fprintf( output, "lload_0\n");
                break;
            case OPCODE_lload_1:
                fprintf( output, "lload_1\n");
                break;
            case OPCODE_lload_2:
                fprintf( output, "lload_2\n");
                break;
            case OPCODE_lload_3:
                fprintf( output, "lload_3\n");
                break;
            case OPCODE_fload_0:
                fprintf( output, "fload_0\n");
                break;
            case OPCODE_fload_1:
                fprintf( output, "fload_1\n");
                break;
            case OPCODE_fload_2:
                fprintf( output, "fload_2\n");
                break;
            case OPCODE_fload_3:
                fprintf( output, "fload_3\n");
                break;
            case OPCODE_dload_0:
                fprintf( output, "dload_0\n");
                break;
            case OPCODE_dload_1:
                fprintf( output, "dload_1\n");
                break;
            case OPCODE_dload_2:
                fprintf( output, "dload_2\n");
                break;
            case OPCODE_dload_3:
                fprintf( output, "dload_3\n");
                break;
            case OPCODE_aload_0:
                fprintf( output, "aload_0\n");
                break;
            case OPCODE_aload_1:
                fprintf( output, "aload_1\n");
                break;
            case OPCODE_aload_2:
                fprintf( output, "aload_2\n");
                break;
            case OPCODE_aload_3:
                fprintf( output, "aload_3\n");
                break;
            case OPCODE_iaload:
                fprintf( output, "iaload\n");
                break;
            case OPCODE_laload:
                fprintf( output, "laload\n");
                break;
            case OPCODE_faload:
                fprintf( output, "faload\n");
                break;
            case OPCODE_daload:
                fprintf( output, "daload\n");
                break;
            case OPCODE_aaload:
                fprintf( output, "aaload\n");
                break;
            case OPCODE_baload:
                fprintf( output, "baload\n");
                break;
            case OPCODE_caload:
                fprintf( output, "caload\n");
                break;
            case OPCODE_saload:
                fprintf( output, "saload\n");
                break;
            case OPCODE_istore:
                fprintf( output, "istore %d\n", code[k+1]);
                k++;
                break;
            case OPCODE_lstore:
                fprintf( output, "lstore %d\n", code[k+1]);
                k++;
                break;
            case OPCODE_fstore:
                fprintf( output, "fstore %d\n", code[k+1]);
                k++;
                break;
            case OPCODE_dstore:
                fprintf( output, "dstore %d\n", code[k+1]);
                k++;
                break;
            case OPCODE_astore:
                fprintf( output, "astore %d\n", code[k+1]);
                k++;
                break;
            case OPCODE_istore_0:
                fprintf( output, "istore_0\n");
                break;
            case OPCODE_istore_1:
                fprintf( output, "istore_1\n");
                break;
            case OPCODE_istore_2:
                fprintf( output, "istore_2\n");
                break;
            case OPCODE_istore_3:
                fprintf( output, "istore_3\n");
                break;
            case OPCODE_lstore_0:
                fprintf( output, "lstore_0\n");
                break;
            case OPCODE_lstore_1:
                fprintf( output, "lstore_1\n");
                break;
            case OPCODE_lstore_2:
                fprintf( output, "lstore_2\n");
                break;
            case OPCODE_lstore_3:
                fprintf( output, "lstore_3\n");
                break;
            case OPCODE_fstore_0:
                fprintf( output, "fstore_0\n");
                break;
            case OPCODE_fstore_1:
                fprintf( output, "fstore_1\n");
                break;
            case OPCODE_fstore_2:
                fprintf( output, "fstore_2\n");
                break;
            case OPCODE_fstore_3:
                fprintf( output, "fstore_3\n");
                break;
            case OPCODE_dstore_0:
                fprintf( output, "dstore_0\n");
                break;
            case OPCODE_dstore_1:
                fprintf( output, "dstore_1\n");
                break;
            case OPCODE_dstore_2:
                fprintf( output, "dstore_2\n");
                break;
            case OPCODE_dstore_3:
                fprintf( output, "dstore_3\n");
                break;
            case OPCODE_astore_0:
                fprintf( output, "astore_0\n");
                break;
            case OPCODE_astore_1:
                fprintf( output, "astore_1\n");
                break;
            case OPCODE_astore_2:
                fprintf( output, "astore_2\n");
                break;
            case OPCODE_astore_3:
                fprintf( output, "astore_3\n");
                break;
            case OPCODE_iastore:
                fprintf( output, "iastore\n");
                break;
            case OPCODE_lastore:
                fprintf( output, "lastore\n");
                break;
            case OPCODE_fastore:
                fprintf( output, "fastore\n");
                break;
            case OPCODE_dastore:
                fprintf( output, "dastore\n");
                break;
            case OPCODE_aastore:
                fprintf( output, "aastore\n");
                break;
            case OPCODE_bastore:
                fprintf( output, "bastore\n");
                break;
            case OPCODE_castore:
                fprintf( output, "castore:\n");
                break;
            case OPCODE_sastore:
                fprintf( output, "sastore\n");
                break;
            case OPCODE_pop:
                fprintf( output, "pop\n");
                break;
            case OPCODE_pop2:
                fprintf( output, "pop2\n");
                break;
            case OPCODE_dup:
                fprintf( output, "dup\n");
                break;
            case OPCODE_dup_x1:
                fprintf( output, "dup_x1\n");
                break;
            case OPCODE_dup_x2:
                fprintf( output, "dup_x2\n");
                break;
            case OPCODE_dup2:
                fprintf( output, "dup2\n");
                break;
            case OPCODE_dup2_x1:
                fprintf( output, "dup2_x1\n");
                break;
            case OPCODE_dup2_x2:
                fprintf( output, "dup2_x2\n");
                break;
            case OPCODE_swap:
                fprintf( output, "swap\n");
                break;
            case OPCODE_iadd:
                fprintf( output, "iadd\n");
                break;
            case OPCODE_ladd:
                fprintf( output, "ladd\n");
                break;
            case OPCODE_fadd:
                fprintf( output, "fadd\n");
                break;
            case OPCODE_dadd:
                fprintf( output, "dadd\n");
                break;
            case OPCODE_isub:
                fprintf( output, "isub\n");
                break;
            case OPCODE_lsub:
                fprintf( output, "lsub\n");
                break;
            case OPCODE_fsub:
                fprintf( output, "fsub\n");
                break;
            case OPCODE_dsub:
                fprintf( output, "dsub:\n");
                break;
            case OPCODE_imul:
                fprintf( output, "imul\n");
                break;
            case OPCODE_lmul:
                fprintf( output, "lmul\n");
                break;
            case OPCODE_fmul:
                fprintf( output, "fmul\n");
                break;
            case OPCODE_dmul:
                fprintf( output, "dmul\n");
                break;
            case OPCODE_idiv:
                fprintf( output, "idiv\n");
                break;
            case OPCODE_ldiv:
                fprintf( output, "ldiv\n");
                break;
            case OPCODE_fdiv:
                fprintf( output, "fdiv\n");
                break;
            case OPCODE_ddiv:
                fprintf( output, "ddiv\n");
                break;
            case OPCODE_irem:
                fprintf( output, "irem\n");
                break;
            case OPCODE_lrem:
                fprintf( output, "lrem\n");
                break;
            case OPCODE_frem:
                fprintf( output, "frem\n");
                break;
            case OPCODE_drem:
                fprintf( output, "drem\n");
                break;
            case OPCODE_ineg:
                fprintf( output, "ineg\n");
                break;
            case OPCODE_lneg:
                fprintf( output, "lneg\n");
                break;
            case OPCODE_fneg:
                fprintf( output, "fneg\n");
                break;
            case OPCODE_dneg:
                fprintf( output, "dneg\n");
                break;
            case OPCODE_ishl:
                fprintf( output, "ishl\n");
                break;
            case OPCODE_lshl:
                fprintf( output, "lshl\n");
                break;
            case OPCODE_ishr:
                fprintf( output, "ishr\n");
                break;
            case OPCODE_lshr:
                fprintf( output, "lshr\n");
                break;
            case OPCODE_iushr:
                fprintf( output, "iushr\n");
                break;
            case OPCODE_lushr:
                fprintf( output, "lushr\n");
                break;
            case OPCODE_iand:
                fprintf( output, "iand\n");
                break;
            case OPCODE_land:
                fprintf( output, "land\n");
                break;
            case OPCODE_ior:
                fprintf( output, "ior\n");
                break;
            case OPCODE_lor:
                fprintf( output, "lor\n");
                break;
            case OPCODE_ixor:
                fprintf( output, "ixor\n");
                break;
            case OPCODE_lxor:
                fprintf( output, "lxor\n");
                break;
            case OPCODE_iinc:
                fprintf( output, "iinc %d %d\n", code[k+1], code[k+2]);
                k=k+2;
                break;
            case OPCODE_i2l:
                fprintf( output, "i2l\n");
                break;
            case OPCODE_i2f:
                fprintf( output, "i2f\n");
                break;
            case OPCODE_i2d:
                fprintf( output, "i2d\n");
                break;
            case OPCODE_l2i:
                fprintf( output, "l2i\n");
                break;
            case OPCODE_l2f:
                fprintf( output, "l2f\n");
                break;
            case OPCODE_l2d:
                fprintf( output, "l2d\n");
                break;
            case OPCODE_f2i:
                fprintf( output, "f2i\n");
                break;
            case OPCODE_f2l:
                fprintf( output, "f2l\n");
                break;
            case OPCODE_f2d:
                fprintf( output, "f2d\n");
                break;
            case OPCODE_d2i:
                fprintf( output, "d2i\n");
                break;
            case OPCODE_d2l:
                fprintf( output, "d2l\n");
                break;
            case OPCODE_d2f:
                fprintf( output, "d2f\n");
                break;
            case OPCODE_i2b:
                fprintf( output, "i2b\n");
                break;
            case OPCODE_i2c:
                fprintf( output, "i2c\n");
                break;
            case OPCODE_i2s:
                fprintf( output, "i2s\n");
                break;
            case OPCODE_lcmp:
                fprintf( output, "lcmp\n");
                break;
            case OPCODE_fcmpl:
                fprintf( output, "fcmpl\n");
                break;
            case OPCODE_fcmpg:
                fprintf( output, "fcmpg\n");
                break;
            case OPCODE_dcmpl:
                fprintf( output, "dcmpl\n");
                break;
            case OPCODE_dcmpg:
                fprintf( output, "dcmpg\n");
                break;
            case OPCODE_ifeq:
                fprintf( output, "ifeq %d %d\n", code[k+1], code[k+2]);
                k=k+2;
                break;
            case OPCODE_ifne:
                fprintf( output, "ifne %d %d\n", code[k+1], code[k+2]);
                k=k+2;
                break;
            case OPCODE_iflt:
                fprintf( output, "iflt %d %d\n", code[k+1], code[k+2]);
                k=k+2;
                break;
            case OPCODE_ifge:
                fprintf( output, "ifge %d %d\n", code[k+1], code[k+2]);
                k=k+2;
                break;
            case OPCODE_ifgt:
                fprintf( output, "ifgt %d %d\n", code[k+1], code[k+2]);
                k=k+2;
                break;
            case OPCODE_ifle:
                fprintf( output, "ifle %d %d\n", code[k+1], code[k+2]);
                k=k+2;
                break;
            case OPCODE_if_icmpeq:
                fprintf( output, "if_icmpeq %d %d\n", code[k+1], code[k+2]);
                k=k+2;
                break;
            case OPCODE_if_icmpne:
                fprintf( output, "if_icmpne %d %d\n", code[k+1], code[k+2]);
                k=k+2;
                break;
            case OPCODE_if_icmplt:
                fprintf( output, "if_icmplt %d %d\n", code[k+1], code[k+2]);
                k=k+2;
                break;
            case OPCODE_if_icmpge:
                fprintf( output, "if_icmpge %d %d\n", code[k+1], code[k+2]);
                k=k+2;
                break;
            case OPCODE_if_icmpgt:
                fprintf( output, "if_icmpgt %d %d\n", code[k+1], code[k+2]);
                k=k+2;
                break;
            case OPCODE_if_icmple:
                fprintf( output, "if_icmple %d %d\n", code[k+1], code[k+2]);
                k=k+2;
                break;
            case OPCODE_if_acmpeq:
                fprintf( output, "if_acmpeq %d %d\n", code[k+1], code[k+2]);
                k=k+2;
                break;
            case OPCODE_if_acmpne:
                fprintf( output, "if_acmpne\n");
                break;
            case OPCODE_goto:
                fprintf( output, "goto %d %d\n", code[k+1], code[k+2]);
                k=k+2;
                break;
            case OPCODE_jsr:
                fprintf( output, "jsr %d %d\n", code[k+1], code[k+2]);
                k=k+2;
                break;
            case OPCODE_ret:
                fprintf( output, "ret %d\n", code[k+1]);
                k++;
                break;
            case OPCODE_tableswitch:
                fprintf( output, "tableswitch\n");
                break;
            case OPCODE_lookupswitch:
                fprintf( output, "lookupswitch\n");
                break;
            case OPCODE_ireturn:
                fprintf( output, "ireturn\n");
                break;
            case OPCODE_lreturn:
                fprintf( output, "lreturn\n");
                break;
            case OPCODE_freturn:
                fprintf( output, "freturn\n");
                break;
            case OPCODE_dreturn:
                fprintf( output, "dreturn\n");
                break;
            case OPCODE_areturn:
                fprintf( output, "areturn\n");
                break;
            case OPCODE_return:
                fprintf( output, "return\n");
                break;
            case OPCODE_getstatic:
                fprintf( output, "getstatic %d %d\n", code[k+1], code[k+2]);
                k=k+2;
                break;
            case OPCODE_putstatic:
                fprintf( output, "putstatic %d %d\n", code[k+1], code[k+2]);
                k=k+2;
                break;
            case OPCODE_getfield:
                fprintf( output, "getfield %d %d\n", code[k+1], code[k+2]);
                k=k+2;
                break;
            case OPCODE_putfield:
                fprintf( output, "putfield %d %d\n", code[k+1], code[k+2]);
                k=k+2;
                break;
            case OPCODE_invokevirtual:
                var_u2 = (code[k+1] << 8) | code[k+2];
                index1 = constant_pool[var_u2].Methodref.class_index;
                index1 = constant_pool[index1].Class.name_index;
                index2 = constant_pool[var_u2].Methodref.name_and_type_index;
                index2 = constant_pool[index2].NameAndType.name_index;
                fprintf( output, "invokevirtual %d <%s.%s>\n", var_u2, constant_pool[index1].UTF8.bytes, constant_pool[index2].UTF8.bytes);
                k=k+2;
                break;
            case OPCODE_invokespecial:
                var_u2 = (code[k+1] << 8) | code[k+2];
                index1 = constant_pool[var_u2].Methodref.class_index;
                index1 = constant_pool[index1].Class.name_index;
                index2 = constant_pool[var_u2].Methodref.name_and_type_index;
                index2 = constant_pool[index2].NameAndType.name_index;
                fprintf( output, "invokespecial %d <%s.%s>\n", var_u2, constant_pool[index1].UTF8.bytes, constant_pool[index2].UTF8.bytes);
                k=k+2;
                break;
            case OPCODE_invokestatic:
                var_u2 = (code[k+1] << 8) | code[k+2];
                index1 = constant_pool[var_u2].Methodref.class_index;
                index1 = constant_pool[index1].Class.name_index;
                index2 = constant_pool[var_u2].Methodref.name_and_type_index;
                index2 = constant_pool[index2].NameAndType.name_index;
                fprintf( output, "invokestatic %d <%s.%s>\n", var_u2, constant_pool[index1].UTF8.bytes, constant_pool[index2].UTF8.bytes);
                k=k+2;
                break;
            case OPCODE_invokeinterface:
                var_u2 = (code[k+1] << 8) | code[k+2];
                fprintf( output, "invokeinterface %d %d %d\n", var_u2, code[k+3], code[k+4]);
                break;
            case OPCODE_new:
                fprintf( output, "new %d %d\n", code[k+1], code[k+2]);
                k=k+2;
                break;
            case OPCODE_newarray:
                fprintf( output, "newarray %d\n", code[k+1]);
                k++;
                break;
            case OPCODE_anewarray:
                fprintf( output, "anewarray %d %d\n", code[k+1], code[k+2]);
                k=k+2;
                break;
            case OPCODE_arraylength:
                fprintf( output, "arraylength\n");
                break;
            case OPCODE_athrow:
                fprintf( output, "athrow\n");
                break;
            case OPCODE_checkcast:
                fprintf( output, "checkcast %d %d\n", code[k+1], code[k+2]);
                k=k+2;
                break;
            case OPCODE_instanceof:
                fprintf( output, "instanceof %d %d\n", code[k+1], code[k+2]);
                k=k+2;
                break;
            case OPCODE_monitorenter:
                fprintf( output, "monitorenter\n");
                break;
            case OPCODE_monitorexit:
                fprintf( output, "monitorexit\n");
                break;
            case OPCODE_wide:
                fprintf( output, "wide\n");
                break;
            case OPCODE_multianewarray:
                fprintf( output, "multianewarray %d %d %d\n", code[k+1], code[k+2], code[k+3]);
                k=k+3;
                break;
            case OPCODE_ifnull:
                fprintf( output, "ifnull %d %d\n", code[k+1], code[k+2]);
                k=k+2;
                break;
            case OPCODE_ifnonnull:
                fprintf( output, "ifnonnull %d %d\n", code[k+1], code[k+2]);
                k=k+2;
                break;
            case OPCODE_goto_w:
                fprintf( output, "goto_w %d %d %d %d\n", code[k+1], code[k+2], code[k+3], code[k+4]);
                break;
            case OPCODE_jsr_w:
                fprintf( output, "jsr_w %d %d %d %d\n", code[k+1], code[k+2], code[k+3], code[k+4]);
                break;
            case OPCODE_breakpoint:
                fprintf( output, "breakpoint\n");
                break;
        }
    }
}

///Debug (sem testar, n�o tenho certeza do funcionamento de nada)
///-----------------------------------------------------------------------------------------------------------------------------
void printFrame(Frame *frame, FILE *output){
    printOperandStack(frame->operandStack, output); /// Funcao para imprimir a pilha de operandos - envia a propria pilha
    //printLocalVariableArray(frame->localVariableArray[0].value->attributes_count, frame->localVariableArray); /// Funcao que imprime o vetor de variaveis locais
    fprintf( output, "Referencia Constant_pool:\t%d", frame->constant_pool);
    //fprintf( output, "Referencia Methods:\t\t%d", frame->methods);
    fprintf( output, "Referencia Handler:\t\t%d", frame->handler);
    fprintf( output, "Return PC:\t\t%d\n", frame->returnPC);
}

///Muito provavelmente n�o funciona. Tem que testar ainda
void printOperandStack(structOperandStack ** operandStack, FILE *output){
    structOperandStack *operandStackTop;
    operandStackTop = *operandStack;
    fprintf( output, "Operand Stack:\n");
    fprintf( output, "---------------------------------------------------------------------------------------\n");
    while(operandStackTop->next != NULL){
        fprintf( output, "%d->", operandStackTop->value);
        operandStackTop = operandStackTop->next;
    }
    fprintf( output, "\n---------------------------------------------------------------------------------------\n");
}

void printLocalVariableArray(u2 attributes_count, LocalVariable *localVariableArray, FILE *output){
    fprintf( output, "Vetor de vari�veis locais:\n");
    fprintf( output, "---------------------------------------------------------------------------------------\n");
    for(int i=0; i<attributes_count; i++){
        fprintf( output, "%d ", localVariableArray[i].value);
    }
    fprintf( output, "\n---------------------------------------------------------------------------------------\n");
}
void printHandler(ClassHandler *handler, FILE *output){
    fprintf( output, "Class Reference:\t%d\n", handler->classRef);
    printFields(handler->classRef->fields_count, handler->fields, output);
}

void printFields(u2 fields_count, field_info *fields, FILE *output){
    fprintf( output, "Fields Vector:\n");
    fprintf( output, "---------------------------------------------------------------------------------------\n");
    for(int i=0; i<fields_count; i++){
        fprintf( output, "Name:\t\tcp_info #%d\n", fields[i].name_index);
        fprintf( output, "Descriptor:\tcp_info #%d\n", fields[i].descriptor_index);
        fprintf( output, "Access Flags:\t\t0x%04x\n", fields[i].access_flags);
        fprintf( output, "Attribute count:\t\t%d\n\n", fields[i].attributes_count);
        ///N�o estou imprimindo os atributos das fields
    }
    fprintf( output, "\n---------------------------------------------------------------------------------------\n");
}
///-----------------------------------------------------------------------------------------------------------------------------
///
