#ifndef JVMMANAGER_H_INCLUDED
#define JVMMANAGER_H_INCLUDED
#ifdef JVMMANAGER_SERV
#define EXTERN
#else
#define EXTERN extern
#endif

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include "classManager.h"
#include "exceptionManager.h"
#include "UserViewer.h"
#include "frameManager.h"

/** \brief Completa o vetor numberOfByteInstruction com a quantidade de bytes necessarias para cada instru��o
 *
 * \param numberOfByteInstruction u1* Vetor que armazena a quantidade de bytes que cada instru��o utiliza.
 * \return void
 *
 */
EXTERN void fillNumberOfByteInstruction(u1 * numberOfByteInstruction);

/** \brief Verifica se o bytecode informado tem o mesmo nome que o indicado dentro do bytecode.
 *
 * \param argv char* nome do bytecode
 * \param class_file ClassFile* espa�o em memoria onde os daados da classe est�o salvos.
 * \return void
 *
 */
EXTERN void verifyClassName(char * argv, ClassFile * class_file);

/** \brief Instancia um novo objeto da classe class_file.
 *
 * \param handler ClassHandler* Objeto que ser� criado.
 * \param numberOfClasses u4* Quantidade de classes instanciadas.
 * \param class_file ClassFile* Classe que ser� instanciada.
 * \return void
 *
 */
EXTERN void createNewObject(ClassHandler * handler, u4 * numberOfClasses,ClassFile * class_file);

/** \brief Instancia um novo Frame de um metodo da classe class_file e logo em seguida o insere no topo da pilha de Frames.
 *
 * \param handler ClassHandler* Objeto dono do metodo.
 * \param method_index u4 Indice do metodo no methods do class_file.
 * \param curPC u4 PC Corrente.
 * \param frameStackTop StructFrameStack* Referencia para o TOPO da pilha de Frames.
 * \return void
 *
 */
EXTERN void createNewFrame(ClassHandler * handler, u4 method_index, u4 curPC,StructFrameStack *frameStackTop);

/** \brief Coloca o frame no topo da pilha de frames.
 *
 * \param frameStackTop StructFrameStack* Referencia para o corrente topo da pilha de frames.
 * \param frame Frame* referencia para o frame que sea inserido no topo da pilha de frames.
 * \return void
 *
 */
EXTERN void pushFrameStack(StructFrameStack *frameStackTop, Frame * frame);

/** \brief Retira um frame do topo da pilha de frames.
 *
 * \param frameStackTop StructFrameStack* Referencia para o TOPO da pilha de frames.
 * \return Frame* Referencia para o frame retirado do topo da pilha.
 *
 */
EXTERN Frame * popFrameStack(StructFrameStack *frameStackTop);

/** \brief Procura pelo metodo main no ClassHandler, caso encontre o metodo main eh criado o Frame dele e inserido no inicio da pilha de frames.
 *
 * \param handler ClassHandler* Referencia ao Objeto.
 * \param curPC u4 PC corrente.
 * \param frameStackTop StructFrameStack* Referencia para o topo da pilha de frames.
 * \return int
 *
 */
EXTERN int createMainFrame(ClassHandler * handler,u4 curPC,StructFrameStack *frameStackTop);



#endif // FRAMEMANAGER_H_INCLUDED