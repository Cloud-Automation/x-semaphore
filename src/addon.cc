#define NAPI_VERSION 3
#include <node_api.h>

#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>

namespace xsemaphore {

napi_value _method_semget(napi_env env, napi_callback_info info) {
    napi_status status;

    size_t argc = 3;
    napi_value argv[3];

    status = napi_get_cb_info(env, info, &argc, argv, NULL, NULL);
    if (argc != 3) {
        napi_throw_error(env, NULL, "Invalid number of arguments.");
    }

    key_t key = 0;
    status = napi_get_value_int32(env, argv[0], &key);
    if (status != napi_ok) {
        napi_throw_error(env, NULL, "Invalid number was passed as argument(0).");
    }

    int nsems = 0;
    status = napi_get_value_int32(env, argv[1], &nsems);
    if (status != napi_ok) {
        napi_throw_error(env, NULL, "Invalid number was passed as arguments(1).");
    }

    int semflg = 0;
    status = napi_get_value_int32(env, argv[2], &semflg);
    if (status != napi_ok) {
        napi_throw_error(env, NULL, "Invalid number was passed as arguments(2).");
    }

    //printf("%d, %d, %d\n", key, nsems, semflg);

    int retValue = semget(key, nsems, semflg);

    napi_value result;
    status = napi_create_int32(env, retValue, &result);

    return result;

}

napi_value _method_semop(napi_env env, napi_callback_info info) {
    napi_status status;

    size_t argc = 2;
    napi_value argv[2];

    status = napi_get_cb_info(env, info, &argc, argv, NULL, NULL); 
    if (argc != 2) {
        napi_throw_error(env, NULL, "Invalid number of arguments.");
    }

    int semid = 0;
    status = napi_get_value_int32(env, argv[0], &semid);
    if (status != napi_ok) {
        napi_throw_error(env, NULL, "Invalid number was passed as argument(0).");
    }


    bool isArray;
    status = napi_is_array(env, argv[1], &isArray);
    if (status != napi_ok || !isArray) 
        napi_throw_error(env, NULL, "Invalid type was passed as argument(1).");

    uint32_t arrayLength = 0;
    status = napi_get_array_length(env, argv[1], &arrayLength);
    if (status != napi_ok || arrayLength == 0)
        napi_throw_error(env, NULL, "Invalid array length, musst be at least of length 1.");

    struct sembuf* sops = (struct sembuf*) calloc(arrayLength, sizeof(struct sembuf));

    for (uint32_t i = 0; i < arrayLength; i += 1) {

        napi_value arrElm;
        status = napi_get_element(env, argv[1], i, &arrElm);
        if (status != napi_ok) napi_throw_error(env, NULL, "Unexpected error.");

        napi_valuetype arrElmType;
        status = napi_typeof(env, arrElm, &arrElmType);
        if (status != napi_ok || arrElmType != napi_object) 
            napi_throw_error(env, NULL, "Invalid type was passed as argument(1).");

        /* sem_num */
        napi_value sem_num_key;
        status = napi_create_string_utf8(env, "sem_num", 7, &sem_num_key);
        if (status != napi_ok) napi_throw_error(env, NULL, "Unexpected error.");

        bool res_sem_num = false;
        status = napi_has_property(env, arrElm, sem_num_key, &res_sem_num);
        if (status != napi_ok) napi_throw_error(env, NULL, "Invalid type was passed as argument(1).");

        if (!res_sem_num) {
            sops[i].sem_num = 0;
        } else {
        
            napi_value n_sem_num;
            status = napi_get_property(env, arrElm, sem_num_key, &n_sem_num);
            if (status != napi_ok) napi_throw_error(env, NULL, "Invalid type was passed as argument(1).");

            int tmpValue;
            status = napi_get_value_int32(env, n_sem_num, &tmpValue);
            if (status != napi_ok) napi_throw_error(env, NULL, "Invalid type was passes as argument(1).");
           
            sops[i].sem_num = tmpValue; 
        
        }

        napi_value sem_op_key;
        status = napi_create_string_utf8(env, "sem_op", 6, &sem_op_key);
        if (status != napi_ok) napi_throw_error(env, NULL, "Unexpected error.");

        bool res_sem_op = false;
        status = napi_has_property(env, arrElm, sem_op_key, &res_sem_op);
        if (status != napi_ok) napi_throw_error(env, NULL, "Invalid type was passed as argument(1).");

        if (!res_sem_op) {
            sops[i].sem_op = 0;
        } else {
        
            napi_value n_sem_op;
            status = napi_get_property(env, arrElm, sem_op_key, &n_sem_op);
            if (status != napi_ok) napi_throw_error(env, NULL, "Invalid type was passed as argument(1).");

            int tmpValue;
            status = napi_get_value_int32(env, n_sem_op, &tmpValue);
            if (status != napi_ok) napi_throw_error(env, NULL, "Invalid type was passes as argument(1).");
           
            sops[i].sem_op = tmpValue; 
        
        }

        /* sem_flg */

        napi_value sem_flg_key;
        status = napi_create_string_utf8(env, "sem_flg", 7, &sem_flg_key);
        if (status != napi_ok) napi_throw_error(env, NULL, "Unexpected error.");

        bool res_sem_flg = false;
        status = napi_has_property(env, arrElm, sem_flg_key, &res_sem_flg);
        if (status != napi_ok) napi_throw_error(env, NULL, "Invalid type was passed as argument(1).");

        if (!res_sem_flg) {
            sops[i].sem_flg = 0;
        } else {
        
            napi_value n_sem_flg;
            status = napi_get_property(env, arrElm, sem_flg_key, &n_sem_flg);
            if (status != napi_ok) napi_throw_error(env, NULL, "Invalid type was passed as argument(1).");

            int tmpValue;
            status = napi_get_value_int32(env, n_sem_flg, &tmpValue);
            if (status != napi_ok) napi_throw_error(env, NULL, "Invalid type was passes as argument(1).");
           
            sops[i].sem_flg = tmpValue; 
        
        }

        //printf("%d : %d, %d, %d\n", i, sops[i].sem_num, sops[i].sem_op, sops[i].sem_flg);

    }
    
    int retValue = semop(semid, sops, arrayLength);
 

    free(sops);

    napi_value result;
    status = napi_create_int32(env, retValue, &result);

    return result;

}

/* 
 * did this so far, but did not need it for myself, feel free to complete! 
napi_value _method_semctl(napi_env env, napi_callback_info info) {
    napi_status status;


    int semid = 0;
    int semnum = 0;
    int cmd = 0;

    size_t argc = 4;
    napi_value argv[4];

    status = napi_get_cb_info(env, info, &argc, argv, NULL, NULL); 
    if (argc < 3) {
        napi_throw_error(env, NULL, "semctl: Invalid number of arguments.");
    }

    status = napi_get_value_int32(env, argv[0], &semid);
    if (status != napi_ok) {
        napi_throw_error(env, NULL, "semctt: Invalid number was passed as argument(0).");
    }

    status = napi_get_value_int32(env, argv[1], &semnum);
    if (status != napi_ok) {
        napi_throw_error(env, NULL, "semctt: Invalid number was passed as argument(1).");
    }

    status = napi_get_value_int32(env, argv[2], &cmd);
    if (status != napi_ok) {
        napi_throw_error(env, NULL, "semctt: Invalid number was passed as argument(2).");
    }

    int retVal = -1;

    switch (cmd) {

        case (IPC_STAT) : {
 
            if (argc != 4) {
                napi_throw_error(env, NULL, "semctl: Invalid number of arguments for SET_VAL.");
            }

            napi_valuetype argValueType;
            status = napi_typeof(env, argv[3], &argValueType);
            if (status != napi_ok || argValueType != napi_object)
                napi_throw_error(env, NULL, "semctl: Argument 4 on Command IPC_STAT must be of type 'object'.");
        
            struct semid_ds data = { };
            retVal = semctl(semid, semnum, IPC_STAT, (struct semid_ds*) &data);

            // create sem_perm object
            napi_value n_sem_perm;
            status = napi_create_object(env, &n_sem_perm);
            if (status != napi_ok) napi_throw_error(env, NULL, "semctl: Unexpected error.");

            // populate sem_perm object
            // __key 
            napi_value n_key;
            status = napi_create_int32(env, data.sem_perm.__key, &n_key);
            if (status != napi_ok) napi_throw_error(env, NULL, "semctl: Unexpected error.");
            status = napi_set_named_property(env, n_sem_perm, "__key", n_key);
            if (status != napi_ok) napi_throw_error(env, NULL, "semctl: Unexpected error.");


            // populate return object
            status = napi_set_named_property(env, argv[3], "sem_perm", n_sem_perm);
            if (status != napi_ok) napi_throw_error(env, NULL, "semctl: Unexpected error.");

            napi_value n_nsems;
            status = napi_create_int32(env, data.sem_nsems, &n_nsems);
            if (status != napi_ok) napi_throw_error(env, NULL, "semctl: Unexpected error.");
            status = napi_set_named_property(env, argv[3], "sem_nsems", n_nsems);
            if (status != napi_ok) napi_throw_error(env, NULL, "semctl: Unexpected error.");

            break;                 
        }

        case (SETVAL) : {

            if (argc != 4) {
                napi_throw_error(env, NULL, "semctl: Invalid number of arguments for SET_VAL.");
            }

            int val = 0;
            status = napi_get_value_int32(env, argv[3], &val);
            if (status != napi_ok) napi_throw_error(env, NULL, "semctl: Unexpecting error.");

            retVal = semctl(semid, semnum, SETVAL, (int) val);

            break;
                        
        } 
    
    }


    napi_value result;
    status = napi_create_int32(env, retVal, &result);
    if (status != napi_ok) napi_throw_error(env, NULL, "Unexpected error.");

    return result;

}
*/

napi_value init(napi_env env, napi_value exports) {
  napi_status status;
  napi_value fn_semget;
  napi_value fn_semop;
  //napi_value fn_semctl;
  napi_value const_sem_undo;
  napi_value const_ipc_creat;
  napi_value const_ipc_excl;
/*  napi_value const_ipc_stat;
  napi_value const_ipc_set;
  napi_value const_ipc_rmid;
  napi_value const_ipc_info;
  napi_value const_sem_info;
  napi_value const_sem_stat;
  napi_value const_getall;
  napi_value const_getncnt;
  napi_value const_getpid;
  napi_value const_getval;
  napi_value const_getzcnt;
  napi_value const_setall;
  napi_value const_setval;
*/
  /* semget */
  status = napi_create_function(env, nullptr, 0, _method_semget, nullptr, &fn_semget);
  if (status != napi_ok) return nullptr;

  status = napi_set_named_property(env, exports, "semget", fn_semget);
  if (status != napi_ok) return nullptr;

  /* semop */
  status = napi_create_function(env, nullptr, 0, _method_semop, nullptr, &fn_semop);
  if (status != napi_ok) return nullptr;

  status = napi_set_named_property(env, exports, "semop", fn_semop);
  if (status != napi_ok) return nullptr;

  /* semctl 
  status = napi_create_function(env, nullptr, 0, _method_semctl, nullptr, &fn_semctl);
  if (status != napi_ok) return nullptr;

  status = napi_set_named_property(env, exports, "semctl", fn_semctl);
  if (status != napi_ok) return nullptr; */

  /* constants */
  /* SEM_UNDO */

  status = napi_create_int32(env, SEM_UNDO, &const_sem_undo);
  if (status != napi_ok) return nullptr;

  status = napi_set_named_property(env, exports, "SEM_UNDO", const_sem_undo);
  if (status != napi_ok) return nullptr;

  /* IPC_CREAT */
  status = napi_create_int32(env, IPC_CREAT, &const_ipc_creat);
  if (status != napi_ok) return nullptr;

  status = napi_set_named_property(env, exports, "IPC_CREAT", const_ipc_creat);
  if (status != napi_ok) return nullptr;

  /* IPC_EXCL */
  status = napi_create_int32(env, IPC_EXCL, &const_ipc_excl);
  if (status != napi_ok) return nullptr;

  status = napi_set_named_property(env, exports, "IPC_EXCL", const_ipc_excl);
  if (status != napi_ok) return nullptr;

  /* IPC_STAT 
  status = napi_create_int32(env, IPC_STAT, &const_ipc_stat);
  if (status != napi_ok) return nullptr;

  status = napi_set_named_property(env, exports, "IPC_STAT", const_ipc_stat);
  if (status != napi_ok) return nullptr;
  */
  /* IPC_SET 
  status = napi_create_int32(env, IPC_SET, &const_ipc_set);
  if (status != napi_ok) return nullptr;

  status = napi_set_named_property(env, exports, "IPC_SET", const_ipc_set);
  if (status != napi_ok) return nullptr;
  */
  /* IPC_RMID 
  status = napi_create_int32(env, IPC_RMID, &const_ipc_rmid);
  if (status != napi_ok) return nullptr;

  status = napi_set_named_property(env, exports, "IPC_RMID", const_ipc_rmid);
  if (status != napi_ok) return nullptr;
  */
  /* IPC_INFO 
  status = napi_create_int32(env, IPC_INFO, &const_ipc_info);
  if (status != napi_ok) return nullptr;

  status = napi_set_named_property(env, exports, "IPC_INFO", const_ipc_info);
  if (status != napi_ok) return nullptr;
  */
  /* SEM_INFO 
  status = napi_create_int32(env, SEM_INFO, &const_sem_info);
  if (status != napi_ok) return nullptr;

  status = napi_set_named_property(env, exports, "SEM_INFO", const_sem_info);
  if (status != napi_ok) return nullptr;
  */
  /* SEM_STAT 
  status = napi_create_int32(env, SEM_STAT, &const_sem_stat);
  if (status != napi_ok) return nullptr;

  status = napi_set_named_property(env, exports, "SEM_STAT", const_sem_stat);
  if (status != napi_ok) return nullptr;
  */
  /* GETALL 
  status = napi_create_int32(env, GETALL, &const_getall);
  if (status != napi_ok) return nullptr;

  status = napi_set_named_property(env, exports, "GETALL", const_getall);
  if (status != napi_ok) return nullptr;
  */
  /* GETNCNT 
  status = napi_create_int32(env, GETNCNT, &const_getncnt);
  if (status != napi_ok) return nullptr;

  status = napi_set_named_property(env, exports, "GETNCNT", const_getncnt);
  if (status != napi_ok) return nullptr;
  */
  /* GETPID 
  status = napi_create_int32(env, GETPID, &const_getpid);
  if (status != napi_ok) return nullptr;

  status = napi_set_named_property(env, exports, "GETPID", const_getpid);
  if (status != napi_ok) return nullptr;
  */
  /* GETVAL 
  status = napi_create_int32(env, GETVAL, &const_getval);
  if (status != napi_ok) return nullptr;

  status = napi_set_named_property(env, exports, "GETVAL", const_getval);
  if (status != napi_ok) return nullptr;
  */
  /* GETZCNT 
  status = napi_create_int32(env, GETZCNT, &const_getzcnt);
  if (status != napi_ok) return nullptr;

  status = napi_set_named_property(env, exports, "GETZCNT", const_getzcnt);
  if (status != napi_ok) return nullptr;
  */
  /* SETALL 
  status = napi_create_int32(env, SETALL, &const_setall);
  if (status != napi_ok) return nullptr;

  status = napi_set_named_property(env, exports, "SETALL", const_setall);
  if (status != napi_ok) return nullptr;
  */
  /* SETVAL 
  status = napi_create_int32(env, SETVAL, &const_setval);
  if (status != napi_ok) return nullptr;

  status = napi_set_named_property(env, exports, "SETVAL", const_setval);
  if (status != napi_ok) return nullptr;
  */

  return exports;

}

NAPI_MODULE(NODE_GYP_MODULE_NAME, init)

}  // namespace demo
