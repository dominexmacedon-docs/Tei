#include "compiler.h"

#include <stdio.h>

static int compile_node(Compiler *compiler, ASTNode *node);

void compiler_init(Compiler *compiler, Chunk *chunk) {
    compiler->chunk = chunk;
}

void compiler_free(Compiler *compiler) {
    (void)compiler;
}

int compiler_compile(Compiler *compiler, ASTNode *node) {
    if (node == NULL) {
        return 0;
    }

    if (!compile_node(compiler, node)) {
        return 0;
    }

    emit_return(compiler, 0);

    return 1;
}

int compiler_compile_statement(Compiler *compiler, ASTNode *node) {
    return compile_node(compiler, node);
}

int compiler_compile_block(Compiler *compiler, ASTNode *node) {
    ASTNode *stmt = node->child;

    while (stmt != NULL) {
        if (!compile_node(compiler, stmt)) {
            return 0;
        }

        stmt = stmt->next;
    }

    return 1;
}

int compiler_compile_expression(Compiler *compiler, ASTNode *node) {
    return compile_node(compiler, node);
}

static int compile_node(Compiler *compiler, ASTNode *node) {
    if (node == NULL) {
        return 1;
    }

    switch (node->type) {

        case AST_NUMBER: {
            double value = *((double *)node->value);
            emit_constant(compiler, value_number(value), node->line);
            return 1;
        }

        case AST_STRING: {
            emit_constant(
                compiler,
                value_string((char *)node->value),
                node->line
            );
            return 1;
        }

        case AST_IDENTIFIER: {
            fprintf(stderr,
                    "Compiler error: identifiers are not implemented yet.\n");
            return 0;
        }

        case AST_NEG_EXPR:
            if (!compile_node(compiler, node->left)) {
                return 0;
            }

            emit_opcode(compiler, OP_NEG, node->line);
            return 1;

        case AST_NOT_EXPR:
            if (!compile_node(compiler, node->left)) {
                return 0;
            }

            emit_opcode(compiler, OP_NOT, node->line);
            return 1;

        case AST_ADD_EXPR:
            if (!compile_node(compiler, node->left)) return 0;
            if (!compile_node(compiler, node->right)) return 0;
            emit_opcode(compiler, OP_ADD, node->line);
            return 1;

        case AST_SUB_EXPR:
            if (!compile_node(compiler, node->left)) return 0;
            if (!compile_node(compiler, node->right)) return 0;
            emit_opcode(compiler, OP_SUB, node->line);
            return 1;

        case AST_MUL_EXPR:
            if (!compile_node(compiler, node->left)) return 0;
            if (!compile_node(compiler, node->right)) return 0;
            emit_opcode(compiler, OP_MUL, node->line);
            return 1;

        case AST_DIV_EXPR:
            if (!compile_node(compiler, node->left)) return 0;
            if (!compile_node(compiler, node->right)) return 0;
            emit_opcode(compiler, OP_DIV, node->line);
            return 1;

        case AST_MOD_EXPR:
            if (!compile_node(compiler, node->left)) return 0;
            if (!compile_node(compiler, node->right)) return 0;
            emit_opcode(compiler, OP_MOD, node->line);
            return 1;

        case AST_EQ_EXPR:
            if (!compile_node(compiler, node->left)) return 0;
            if (!compile_node(compiler, node->right)) return 0;
            emit_opcode(compiler, OP_EQUAL, node->line);
            return 1;

        case AST_NEQ_EXPR:
            if (!compile_node(compiler, node->left)) return 0;
            if (!compile_node(compiler, node->right)) return 0;
            emit_opcode(compiler, OP_NOT_EQUAL, node->line);
            return 1;

        case AST_GT_EXPR:
            if (!compile_node(compiler, node->left)) return 0;
            if (!compile_node(compiler, node->right)) return 0;
            emit_opcode(compiler, OP_GREATER, node->line);
            return 1;

        case AST_GTE_EXPR:
            if (!compile_node(compiler, node->left)) return 0;
            if (!compile_node(compiler, node->right)) return 0;
            emit_opcode(compiler, OP_GREATER_EQUAL, node->line);
            return 1;

        case AST_LT_EXPR:
            if (!compile_node(compiler, node->left)) return 0;
            if (!compile_node(compiler, node->right)) return 0;
            emit_opcode(compiler, OP_LESS, node->line);
            return 1;

        case AST_LTE_EXPR:
            if (!compile_node(compiler, node->left)) return 0;
            if (!compile_node(compiler, node->right)) return 0;
            emit_opcode(compiler, OP_LESS_EQUAL, node->line);
            return 1;

        case AST_SHOW_STMT:
            if (!compile_node(compiler, node->left)) {
                return 0;
            }

            emit_opcode(compiler, OP_SHOW, node->line);
            return 1;

        case AST_BLOCK:
            return compiler_compile_block(compiler, node);

        case AST_LET_STMT:
            fprintf(stderr,
                    "Compiler error: let statements are not implemented yet.\n");
            return 0;

        case AST_ASSIGN_EXPR:
            fprintf(stderr,
                    "Compiler error: assignment is not implemented yet.\n");
            return 0;

        case AST_IF_STMT:
            fprintf(stderr,
                    "Compiler error: if statements are not implemented yet.\n");
            return 0;

        case AST_WHILE_STMT:
            fprintf(stderr,
                    "Compiler error: while statements are not implemented yet.\n");
            return 0;

        case AST_FUNCTION:
            fprintf(stderr,
                    "Compiler error: functions are not implemented yet.\n");
            return 0;

        case AST_ERROR:
            fprintf(stderr,
                    "Compiler error: invalid AST.\n");
            return 0;

        default:
            fprintf(stderr,
                    "Compiler error: unknown AST node type %d.\n",
                    node->type);
            return 0;
    }
}
