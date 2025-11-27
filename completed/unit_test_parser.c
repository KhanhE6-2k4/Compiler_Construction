/* to build test file:
    - go to folder Bai2/completed
    - run the following command:
      gcc -I. -I/opt/homebrew/include -L/opt/homebrew/lib -o unit_test_parser unit_test_parser.c reader.c charcode.c token.c error.c scanner.c -lcmocka -Wl,--wrap=exit

*/

/* run test file:
    ./unit_test_parser
*/

#include <stdarg.h>
#include <stddef.h>
#include <setjmp.h> 
#include <cmocka.h>

#include <stdio.h>
#include <stdlib.h>
// 
#include "reader.h"
#include "charcode.h"
#include "token.h"
#include "error.h"
#include "scanner.h"
#include "parser.c"

#include <string.h>
#include <stdlib.h>
#include <assert.h>

#include <setjmp.h>



// #include "main.h"


extern int lineNo;
extern int colNo;
extern int currentChar;

extern CharCode charCodes[];

// p2() goi p1()
// p1() goi exit
// Cau lenh tiep theo sau p1() trong p2()
// __wrap_exit return về đây
// Code tiếp tục chạy! -> VẤN ĐỀ
// Cần xử lý bằng cách dưới đây
// Sử dụng: setjmp và longjmp -cho phép "nhảy" từ một chỗ trong code sang chỗ khác
// **Có longjmp** (hoạt động tốt):
// setjmp() → compile() → error() → __wrap_exit() → longjmp() → NHẢY về setjmp → tiếp tục an toàn

// !!! CHÚ Ý !!!
// longjmp(exit_jmp, 0);  // ta truyền vào 0
// Nhưng khi nhảy về:
// int exit_status = setjmp(exit_jmp);  (*)
// C tự động return 1, không phải 0
// Mục đích của việc này là để để phân biệt lần gọi đầu tiên và lần nhảy về đvs (*)


static jmp_buf exit_jmp; // lưu "địa chỉ" để nhảy về
static int exit_called = 0; // Dùng để có debug sau này

void __wrap_exit(int status) {
    function_called();
    check_expected(status);
    exit_called = 1;
    longjmp(exit_jmp, status);  // Jump back to setjmp
}


 void strip_newline(char *s) {
    size_t len = strlen(s);
    while (len > 0 && (s[len-1] == '\n' || s[len-1] == '\r')) {
        s[len-1] = '\0';
        len--;
    }
}

static void compare_file_contents(const char *file1, const char *file2) {
    FILE *f1 = fopen(file1, "r");
    FILE *f2 = fopen(file2, "r");
    assert_non_null(f1);
    assert_non_null(f2);

    char line1[1024];
    char line2[1024];

    while (fgets(line1, sizeof(line1), f1) && fgets(line2, sizeof(line2), f2)) {
        strip_newline(line1);
        strip_newline(line2);
        assert_string_equal(line1, line2);
    }

    fclose(f1);
    fclose(f2);
}

/* test input file 1*/
static void test_scan(char *input_filename, char *expected_output_filename) {
    // (void) state; /* unused */
    char temp_filename[] = "temp.txt";
    int status;
    
    FILE *temp_file = freopen(temp_filename, "w", stdout);
    assert_non_null(temp_file);

    // Set up jump point for exit()
    int exit_status = setjmp(exit_jmp);
    
    if (exit_status == 0) {
        // First time through - run the compile
        status = compile(input_filename);
        assert_int_equal(status, IO_SUCCESS);
    }
    else {
        // Returned from longjmp (exit was called)
        // This is expected for error cases
    }
    fclose(temp_file);
    
    compare_file_contents(temp_filename, expected_output_filename);
    
    remove(temp_filename);
}


static void test_input1(void **state) {
    (void) state; /* unused */
    test_scan("../test/example1.kpl", "../test/result1.txt");
    assert_true(currentChar == EOF);
}

static void test_input2(void **state) {
    (void) state; /* unused */
    test_scan("../test/example2.kpl", "../test/result2.txt");
    assert_true(currentChar == EOF);
}

static void test_input3(void **state) {
    (void) state; /* unused */
    test_scan("../test/example3.kpl", "../test/result3.txt");
    assert_true(1);
}

static void test_input4(void **state) {
    (void) state; /* unused */
    test_scan("../test/example4.kpl", "../test/result4.txt");
    assert_true(1);
}

static void test_input5(void **state) {
    (void) state; /* unused */
    expect_function_call(__wrap_exit);
    expect_value(__wrap_exit, status, 0); // because error() use exit(0)
    test_scan("../test/example5.kpl", "../test/result5.txt");
    assert_true(1);
}

static void test_input6(void **state) {
    (void) state; /* unused */
    expect_function_call(__wrap_exit);
    expect_value(__wrap_exit, status, 0); // because error() use exit(0)
    test_scan("../test/example6.kpl", "../test/result6.txt");
    assert_true(1);
}

static void test_input7(void **state) {
    (void) state; /* unused */
    expect_function_call(__wrap_exit);
    expect_value(__wrap_exit, status, 0); // because error() use exit(0)
    test_scan("../test/example7.kpl", "../test/result7.txt");
    assert_true(1);
}



int main(void) {
    // cmocka_set_message_output(CM_OUTPUT_TAP);
    
    const struct CMUnitTest tests[] = {
        cmocka_unit_test(test_input1),
        cmocka_unit_test(test_input2),
        cmocka_unit_test(test_input3),
        cmocka_unit_test(test_input4),
        cmocka_unit_test(test_input5),
        cmocka_unit_test(test_input6),
        cmocka_unit_test(test_input7),
    };
    return cmocka_run_group_tests(tests, NULL, NULL);
}