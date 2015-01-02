#ifndef __DYNAMIC_LOADER_H_
#define __DYNAMIC_LOADER_H_

#define FAILURE 1
#define SUCCESS 0

// Simple pass through to let the implementation of error reporting be changed everywhere without modifying the code below.
#define log_error(args...) printf(args)


/**
 * Takes a handle to a dynamically loaded shared object, 
 * a string corresponding to the desired function name in the shared object, 
 * and an opaque function pointer. The function pointer is populated with the
 * address of the function in the handle as the result of the dlsym() operation. 
 *
 * If an error is encountered, FAILURE is returned, otherwise SUCCESS.
 */
int load_fn_from_shared_object(void *handle, const char *fn_name, void **fn_ptr);

/**
 * Takes a handle pointer pointer to a dynamically loaded shared object,
 * and a name of the shared object to be loaded.
 *
 * If the dereference of handle is not NULL, the handle is dlclosed(), thus decrementing
 * the reference counter. The handle is then repopulated with the result
 * of reloading the shared object into the handle. 
 *
 * @param so_name Path to a shared object library.
 * @param handle Handle pointer pointer of the shared object to refresh.
 *
 * @return if an error is encountered, FAILURE, otherwise SUCCESS. 
 */
int load_shared_object(const char *so_name, void **handle);

/**
 * Takes a handle to a loaded shared object, and closes it if the handle is not NULL. 
 *
 * @param handle Handle of the shared object to dlclose(). 
 */
void unload_shared_object(void **handle);

/**
 * Runs the function specified by fn_name, which is dynamically resolved from the library specified by so_name. The resulting function
 * pointer is executed with args and result pointers as parameters. 
 *
 * @param so_name Path to a shared object library.
 * @param fn_name name of a function in the library specified by so_name to load.
 * @param args an opaque pointer to be passed to the function pointer returned by resolving fn_name.
 * @param args an opaque pointer for the result of calling the function pointer returned by resolving fn_name.
 *
 * @return SUCCESS if errno was not set and the function returned SUCCESS, and FAILURE otherwise.
 */
int exec_dynamic_function(const char *so_name, const char *fn_name, void *args, void *result);

#endif
