#include <stdio.h>

#include "../src/compiler/compiler.h"
#include "../src/parser/parser.h"
#include "../src/vm/vm.h"
#include "../src/vm/chunk.h"
#include "../src/vm/value.h"
#include "../src/vm/opcode.h"

static void run_test(const char *name, ASTNode *ast) {
    printf("========== %s ==========\n", name);

    Chunk chunk;
    chunk_init(&chunk);

    Compiler compiler;
    compiler_init(&compiler, &chunk);

    if (!compiler_compile(&compiler, ast)) {
        printf("Compilation failed\n");
        chunk_free(&chunk);
        return;
    }

    VM vm;
    vm_init(&vm);

    VMResult result = vm_interpret(&vm, &chunk);

    printf("\nVM Result: %s\n", result == VM_OK ? "OK" : "ERROR");

    vm_free(&vm);
    chunk_free(&chunk);
}

static ASTNode *number(double v) {
    ASTNode *n = ast_create_node(AST_NUMBER);

    double *val = malloc(sizeof(double));
    *val = v;

    n->value = val;
    return n;
}

static ASTNode *binary(ASTNodeType op, ASTNode *a, ASTNode *b) {
    ASTNode *n = ast_create_node(op);
    n->left = a;
    n->right = b;
    return n;
}

static ASTNode *show(ASTNode *expr) {
    ASTNode *n = ast_create_node(AST_SHOW_STMT);
    n->left = expr;
    return n;
}

int main(void) {

    ASTNode *expr1 =
        show(
            binary(
                AST_ADD_EXPR,
                number(10),
                number(20)
            )
        );

    ASTNode *expr2 =
        show(
            binary(
                AST_MUL_EXPR,
                binary(AST_ADD_EXPR, number(2), number(3)),
                number(4)
            )
        );

    ASTNode *expr3 =
        show(
            binary(
                AST_SUB_EXPR,
                binary(AST_MUL_EXPR, number(10), number(5)),
                number(20)
            )
        );

    run_test("TEST 1: 10 + 20", expr1);
    run_test("TEST 2: (2 + 3) * 4", expr2);
    run_test("TEST 3: (10 * 5) - 20", expr3);

    return 0;
}
