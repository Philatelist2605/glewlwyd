/**
 *
 * Glewlwyd SSO Server
 *
 * Authentiation server
 * Users are authenticated via various backend available: database, ldap
 * Using various authentication methods available: password, OTP, send code, etc.
 * 
 * Declarations for common constants and prototypes used in Glewlwyd main program and modules
 *
 * Copyright 2016-2019 Nicolas Mora <mail@babelouest.org>
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU GENERAL PUBLIC LICENSE
 * License as published by the Free Software Foundation;
 * version 3 of the License.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU GENERAL PUBLIC LICENSE for more details.
 *
 * You should have received a copy of the GNU General Public
 * License along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 */

#ifndef __GLEWLWYD_COMMON_H_
#define __GLEWLWYD_COMMON_H_

#include <jansson.h>

#include <ulfius.h>
#include <yder.h>
#include <hoel.h>

#include "static_file_callback.h"

#define G_OK                 0
#define G_ERROR              1
#define G_ERROR_UNAUTHORIZED 2
#define G_ERROR_PARAM        3
#define G_ERROR_DB           4
#define G_ERROR_MEMORY       5
#define G_ERROR_NOT_FOUND    6

// Callback priority
#define GLEWLWYD_CALLBACK_PRIORITY_ZERO           0
#define GLEWLWYD_CALLBACK_PRIORITY_AUTHENTICATION 1
#define GLEWLWYD_CALLBACK_PRIORITY_APPLICATION    2
#define GLEWLWYD_CALLBACK_PRIORITY_CLOSE          3
#define GLEWLWYD_CALLBACK_PRIORITY_PLUGIN         4
#define GLEWLWYD_CALLBACK_PRIORITY_FILE           100
#define GLEWLWYD_CALLBACK_PRIORITY_GZIP           101

#define GLEWLWYD_IS_VALID_MODE_ADD            0
#define GLEWLWYD_IS_VALID_MODE_UPDATE         1
#define GLEWLWYD_IS_VALID_MODE_UPDATE_PROFILE 2

#define GLEWLWYD_IS_NOT_AVAILABLE 0
#define GLEWLWYD_IS_AVAILABLE     1
#define GLEWLWYD_IS_REGISTERED    2

typedef enum {
  digest_SHA1,
  digest_SSHA1,
  digest_SHA224,
  digest_SSHA224,
  digest_SHA256,
  digest_SSHA256,
  digest_SHA384,
  digest_SSHA384,
  digest_SHA512,
  digest_SSHA512,
  digest_MD5,
  digest_SMD5,
} digest_algorithm;

struct config_module;

struct _user_module {
  void     * file_handle;
  char     * name;
  char     * display_name;
  char     * description;
  char     * parameters;
  int     (* user_module_load)(struct config_module * config, char ** name, char ** display_name, char ** description, char ** parameters);
  int     (* user_module_unload)(struct config_module * config);
  int     (* user_module_init)(struct config_module * config, const char * parameters, void ** cls);
  int     (* user_module_close)(struct config_module * config, void * cls);
  size_t  (* user_module_count_total)(const char * pattern, void * cls);
  char *  (* user_module_get_list)(const char * pattern, size_t offset, size_t limit, int * result, void * cls);
  char *  (* user_module_get)(const char * username, int * result, void * cls);
  char *  (* user_module_get_profile)(const char * username, int * result, void * cls);
  char *  (* user_is_valid)(const char * username, const char * str_user, int mode, int * result, void * cls);
  int     (* user_module_add)(const char * str_new_user, void * cls);
  int     (* user_module_update)(const char * username, const char * str_user, void * cls);
  int     (* user_module_update_profile)(const char * username, const char * str_user, void * cls);
  int     (* user_module_delete)(const char * username, void * cls);
  int     (* user_module_check_password)(const char * username, const char * password, void * cls);
  int     (* user_module_update_password)(const char * username, const char * new_password, void * cls);
};

struct _user_module_instance {
  char                * name;
  struct _user_module * module;
  void                * cls;
  short int             enabled;
  short int             readonly;
};

struct _client_module {
  void     * file_handle;
  char     * name;
  char     * display_name;
  char     * description;
  char     * parameters;
  int     (* client_module_load)(struct config_module * config, char ** name, char ** display_name, char ** description, char ** parameters);
  int     (* client_module_unload)(struct config_module * config);
  int     (* client_module_init)(struct config_module * config, const char * parameters, void ** cls);
  int     (* client_module_close)(struct config_module * config, void * cls);
  size_t  (* client_module_count_total)(const char * pattern, void * cls);
  char  * (* client_module_get_list)(const char * pattern, size_t offset, size_t limit, int * result, void * cls);
  char  * (* client_module_get)(const char * client_id, int * result, void * cls);
  char *  (* client_is_valid)(const char * username, const char * str_user, int mode, int * result, void * cls);
  int     (* client_module_add)(const char * str_new_client, void * cls);
  int     (* client_module_update)(const char * client_id, const char * str_client, void * cls);
  int     (* client_module_delete)(const char * client_id, void * cls);
  int     (* client_module_check_password)(const char * client_id, const char * password, void * cls);
};

struct _client_module_instance {
  char                  * name;
  struct _client_module * module;
  void                  * cls;
  short int               enabled;
  short int               readonly;
};

struct _user_auth_scheme_module {
  void * file_handle;
  char * name;
  char * display_name;
  char * description;
  char * parameters;
  int (* user_auth_scheme_module_load)(struct config_module * config, char ** name, char ** display_name, char ** description, char ** parameters);
  int (* user_auth_scheme_module_unload)(struct config_module * config);
  int (* user_auth_scheme_module_init)(struct config_module * config, const char * parameters, void ** cls);
  int (* user_auth_scheme_module_close)(struct config_module * config, void * cls);
  int (* user_auth_scheme_module_trigger)(const char * username, const char * scheme_trigger, char ** scheme_trigger_response, void * cls);
  int (* user_auth_scheme_module_register)(const char * username, const char * scheme_data, char ** scheme_data_response, void * cls);
  int (* user_auth_scheme_module_validate)(const char * username, const char * scheme_data, void * cls);
  int (* user_can_use_scheme)(const char * username, void * cls);
};

struct _user_auth_scheme_module_instance {
  char                            * name;
  struct _user_auth_scheme_module * module;
  json_int_t                        guasmi_id;
  json_int_t                        guasmi_expiration;
  json_int_t                        guasmi_max_use;
  void                            * cls;
  short int                         enabled;
};

// mock declaration
struct config_plugin;

struct _plugin_module {
  void * file_handle;
  char * name;
  char * display_name;
  char * description;
  char * parameters;
  int (* plugin_module_load)(struct config_plugin * config, char ** name, char ** display_name, char ** description, char ** parameters);
  int (* plugin_module_unload)(struct config_plugin * config);
  int (* plugin_module_init)(struct config_plugin * config, const char * parameters, void ** cls);
  int (* plugin_module_close)(struct config_plugin * config, void * cls);
};

struct _plugin_module_instance {
  char                  * name;
  struct _plugin_module * module;
  void                  * cls;
  short int               enabled;
};

struct config_elements {
  char *                                      config_file;
  unsigned int                                port;
  char *                                      external_url;
  char *                                      api_prefix;
  unsigned long                               log_mode;
  unsigned long                               log_level;
  char *                                      log_file;
  struct _static_file_config *                static_file_config;
  char *                                      admin_scope;
  char *                                      profile_scope;
  char *                                      allow_origin;
  unsigned int                                use_secure_connection;
  char *                                      secure_connection_key_file;
  char *                                      secure_connection_pem_file;
  struct _h_connection *                      conn;
  struct _u_instance *                        instance;
  char *                                      session_key;
  unsigned int                                session_expiration;
  unsigned int                                salt_length;
  digest_algorithm                            hash_algorithm;
  char *                                      login_url;
  char *                                      user_module_path;
  struct _pointer_list *                      user_module_list;
  struct _pointer_list *                      user_module_instance_list;
  char *                                      client_module_path;
  struct _pointer_list *                      client_module_list;
  struct _pointer_list *                      client_module_instance_list;
  char *                                      user_auth_scheme_module_path;
  struct _pointer_list *                      user_auth_scheme_module_list;
  struct _pointer_list *                      user_auth_scheme_module_instance_list;
  char *                                      plugin_module_path;
  struct _pointer_list *                      plugin_module_list;
  struct _pointer_list *                      plugin_module_instance_list;
  struct config_plugin *                      config_p;
  struct config_module *                      config_m;
};

struct config_plugin {
  struct config_elements * glewlwyd_config;
  int      (* glewlwyd_callback_add_plugin_endpoint)(struct config_plugin * config, const char * method, const char * prefix, const char * url, unsigned int priority, int (* callback)(const struct _u_request * request, struct _u_response * response, void * user_data), void * user_data);
  int      (* glewlwyd_callback_remove_plugin_endpoint)(struct config_plugin * config, const char * method, const char * prefix, const char * url);
  json_t * (* glewlwyd_callback_check_session_valid)(struct config_plugin * config, const struct _u_request * request, const char * scope_list);
  json_t * (* glewlwyd_callback_check_user_valid)(struct config_plugin * config, const char * username, const char * password, const char * scope_list);
  json_t * (* glewlwyd_callback_check_client_valid)(struct config_plugin * config, const char * client_id, const char * password, const char * scope_list);
  json_t * (* glewlwyd_callback_get_client_granted_scopes)(struct config_plugin * config, const char * client_id, const char * username, const char * scope_list);
  int      (* glewlwyd_callback_trigger_session_used)(struct config_plugin * config, const struct _u_request * request, const char * scope_list);
  char   * (* glewlwyd_callback_get_plugin_external_url)(struct config_plugin * config, const char * name);
  char   * (* glewlwyd_callback_get_login_url)(struct config_plugin * config, const char * client_id, const char * scope_list, const char * callback_url);
  char   * (* glewlwyd_callback_generate_hash)(struct config_plugin * config, const char * data);
  
  // Client CRUD
  json_t * (* glewlwyd_callback_get_user_list)(struct config_plugin * config, const char * pattern, size_t offset, size_t limit);
  json_t * (* glewlwyd_callback_get_user)(struct config_plugin * config, const char * username);
  json_t * (* glewlwyd_callback_get_user_profile)(struct config_plugin * config, const char * username);
  int (* add_user)(struct config_plugin * config, json_t * j_user);
  int (* set_user)(struct config_plugin * config, const char * username, json_t * j_user);
  int (* delete_user)(struct config_plugin * config, const char * username);
};

struct config_module {
  const char              * external_url;
  const char              * login_url;
  const char              * admin_scope;
  const char              * profile_scope;
  struct _h_connection    * conn;
  digest_algorithm          hash_algorithm;
  struct config_elements  * glewlwyd_config;
  char                 * (* glewlwyd_callback_get_user)(struct config_module * config, const char * username, int * result);
  int                    (* glewlwyd_callback_set_user)(struct config_module * config, const char * username, const char * str_user);
};

// Misc functions
const char * get_ip_source(const struct _u_request * request);
char * get_client_hostname(const struct _u_request * request);
unsigned char random_at_most(unsigned char max);
char * rand_string(char * str, unsigned char str_size);
char * join_json_string_array(json_t * j_array, const char * separator);
char * url_encode(const char * str);
int generate_digest(digest_algorithm digest, const char * password, int use_salt, char * out_digest);
char * generate_hash(digest_algorithm digest, const char * password);
/**
 * Check if the result json object has a "result" element that is equal to value
 */
int check_result_value(json_t * result, const int value);

#endif
