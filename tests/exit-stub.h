#pragma once
#include <assert.h>
#include <unistd.h>
#include <setjmp.h>

int expected_code = 0;
int should_exit = 0;
int done = 0;
jmp_buf jump_env = {};
void exit(int);
void reset_exit_stub();

// stub exit
void exit(int code)
{
  if (!done)
  {
    assert(should_exit == 1);
    assert(expected_code == code);
    longjmp(jump_env, 1);
  }
  else
  {
    _exit(code);
  }
}

void reset_exit_stub()
{
  done = 1;
  should_exit = 0;
  expected_code = 0;
}