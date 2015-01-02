#include <unistd.h>
#include <stdio.h>
#include <dlfcn.h>
#include <errno.h>
#include <stdlib.h>
#include "dynamic_loader.h"

int load_fn_from_shared_object(void *handle, const char *fn_name, void **fn_ptr)
{
  dlerror(); // Clear any errors outstanding.

  *fn_ptr = dlsym(handle, fn_name);

  if (__builtin_expect(NULL != dlerror(), 0)) {
    log_error("Failed to load function %s. Error: %s\n", fn_name, dlerror());
    return FAILURE;
  }

  return SUCCESS;
}

int load_shared_object(const char *so_name, void **handle)
{
  // Refresh the handle with the pointer to the newly loaded library.
  *handle = dlopen(so_name, RTLD_LAZY|RTLD_LOCAL);

  if (__builtin_expect(NULL == handle, 0)) {
    log_error("Failed to load dynamic library: %s error: %s\n", so_name, dlerror());
    return FAILURE;
  }

  return SUCCESS;
}

void unload_shared_object(void **handle)
{
  if (__builtin_expect(NULL != *handle, 1)) {
    dlclose(*handle);
  }
}

int exec_dynamic_function(const char *so_name, const char *fn_name, void *args, void *result)
{
  void *handle = NULL;
  int ret_code;
  // we can specify the the function signature here, but be careful not to
  // cast 
  int (*fn)(void *, void *) = NULL;
  errno = 0;

  load_shared_object(so_name, &handle);
  // Load the function from the shared library context stored in handle
  load_fn_from_shared_object(handle, fn_name, (void **) &fn);

  // Run it!
  ret_code = fn(args, result);

  // It is necessary to unload the library via dlclose() so that the reference count drops to zero,
  // and the library is removed from memory. If this step is ignored, subsequent dlopen calls will not load
  // the library again, but will simply increment the reference count. 
  unload_shared_object(&handle);

  return errno|ret_code ? FAILURE : SUCCESS;
}

int main(int argc, char** argv)
{
  const char *so_name = "./loaded_module.so";
  const char *fn_name = "__dynamic_fn_entry_point__";
  int args = atoi(argv[1]);
  int result = 0;

  while (1) {
    // Because the library is reloaded each time exec_dynamic_function is called (not efficient, but illustrative of what you can do), 
    // you can recompile the library specified by so_name above and any changes made to the function specified by fn_name will be picked
    // up on the next execution. This means that you can hot swap new code into running processes. Woot! 
    exec_dynamic_function(so_name, fn_name, (void *) &args, (void *) &result);
    printf("Result in main: %i\n", result);
    sleep(2);
  }
  return 0;
}
