/**
 * This program implements a GObject for a simple class, then
 * registers the object on the D-Bus and starts serving requests.
 *
 * This maemo code example is licensed under a MIT-style license,
 * that can be found in the file called "License" in the same
 * directory as this file.
 * Copyright (c) 2007 Nokia Corporation. All rights reserved.
 *
 * In more complicated code, the GObject definition, implementation
 * and D-Bus registration would all live in separate files. In this
 * server, they're all present in this file.
 *
 * This program will pull the automatically generated D-Bus/GLib stub
 * code (which contains the marshaling code as well as a binding/call
 * table.
 *
 * This program also might serve as an introduction into implementing
 * custom GType/GObjects, but it is not the primary purpose here.
 * Important things like object life-time management (freeing of
 * objects), sub-classing and GObject properties are not covered at
 * all.
 *
 * Demonstrates a simple implementation of "tracing" as well (via the
 * NO_DAEMON define, as when built as non-daemonizing version, will
 * output information about what is happening and where. Adding
 * timestamps to each trace message is left out (see gettimeofday()
 * system call for a simple solution).
 */

#include <glib.h>
#include <dbus/dbus-glib.h>
#include <stdlib.h> /* exit, EXIT_FAILURE */
#include <unistd.h> /* daemon */

/* Pull symbolic constants that are shared (in this example) between
   the client and the server. */
#include "common-defs.h"




/* This defines the per-instance state.

   Each GObject must start with the 'parent' definition so that common
   operations that all GObjects support can be called on it. */
typedef struct {
  /* The parent class object state. */
  GObject parent;
  /* Our first per-object state variable. */
  gboolean value1;
  /* Our second per-object state variable. */

} ValueObject;

/* Per class state.

   For the first Value implementation we only have the bare minimum,
   that is, the common implementation for any GObject class. */
typedef struct {
  /* The parent class state. */
  GObjectClass parent;
} ValueObjectClass;

/* Forward declaration of the function that will return the GType of
   the Value implementation. Not used in this program since we only
   need to push this over the D-Bus. */
/* Forward declaration of the function that will return the GType of
   the Value implementation. Not used in this program since we only
   need to push this over the D-Bus. */
GType value_object_get_type(void);

/* Macro for the above. It is common to define macros using the
   naming convention (seen below) for all GType implementations,
   and that's why we're going to do that here as well. */
#define VALUE_TYPE_OBJECT (value_object_get_type())

#define VALUE_OBJECT(object) \
        (G_TYPE_CHECK_INSTANCE_CAST((object), \
         VALUE_TYPE_OBJECT, ValueObject))
#define VALUE_OBJECT_CLASS(klass) \
        (G_TYPE_CHECK_CLASS_CAST((klass), \
         VALUE_TYPE_OBJECT, ValueObjectClass))
#define VALUE_IS_OBJECT(object) \
        (G_TYPE_CHECK_INSTANCE_TYPE((object), \
         VALUE_TYPE_OBJECT))
#define VALUE_IS_OBJECT_CLASS(klass) \
        (G_TYPE_CHECK_CLASS_TYPE((klass), \
         VALUE_TYPE_OBJECT))
#define VALUE_OBJECT_GET_CLASS(obj) \
        (G_TYPE_INSTANCE_GET_CLASS((obj), \
         VALUE_TYPE_OBJECT, ValueObjectClass))

/* Utility macro to define the value_object GType structure. */
G_DEFINE_TYPE(ValueObject, value_object, G_TYPE_OBJECT)

gboolean value_object_service_state(ValueObject* obj, gboolean* value_out,
                                                  GError** error);

/**
 * Pull in the stub for the server side.
 */
#include "fms-server-stub.h"

  /*... Listing cut for brevity ...*/

/**
 * Per object initializer
 *
 * Only sets up internal state (both values set to zero)
 */
static void value_object_init(ValueObject* obj) {
  //dbg("Called");

  g_assert(obj != NULL);

  obj->value1 = FALSE;
  
}
/**
 * Per class initializer
 *
 * Registers the type into the GLib/D-Bus wrapper so that it may add
 * its own magic.
 */
static void value_object_class_init(ValueObjectClass* klass) {

 // dbg("Called");

  g_assert(klass != NULL);

  //dbg("Binding to GLib/D-Bus");

  /* Time to bind this GType into the GLib/D-Bus wrappers.
     NOTE: This is not yet "publishing" the object on the D-Bus, but
           since it is only allowed to do this once per class
           creation, the safest place to put it is in the class
           initializer.
           Specifically, this function adds "method introspection
           data" to the class so that methods can be called over
           the D-Bus. */
  dbus_g_object_type_install_info(VALUE_TYPE_OBJECT,
                                 &dbus_glib_value_object_object_info);

 // dbg("Done");
  /* All done. Class is ready to be used for instantiating objects */
}

/**
 * Function that gets called when someone tries to execute "setvalue1"
 * over the D-Bus. (Actually the marshaling code from the stubs gets
 * executed first, but they will eventually execute this function.)
 *
 * NOTE: If you change the name of this function, the generated
 *       stubs will no longer find it! On the other hand, if you
 *       decide to modify the interface XML, this is one of the places
 *       that you'll have to modify as well.
 *       This applies to the next four functions (including this one).
 */
gboolean value_object_service_state(ValueObject* obj, gboolean* value_out, GError** error) {

 // dbg("Called (valueIn=%d)", value_out);

  g_assert(obj != NULL);

  /* Change the value. */
  obj->value1 = TRUE;

  /* Return success to GLib/D-Bus wrappers. In this case we don't need
     to touch the supplied error pointer-pointer. */
  return TRUE;
}


#include "common-defs.h"

  /*... Listing cut for brevity ...*/

int main(int argc, char** argv) {
  /* The GObject representing a D-Bus connection. */
  DBusGConnection* bus = NULL;
  /* Proxy object representing the D-Bus service object. */
  DBusGProxy* busProxy = NULL;
  /* Will hold one instance of ValueObject that will serve all the
     requsts. */
  ValueObject* valueObj = NULL;
  /* GMainLoop for our program. */
  GMainLoop* mainloop = NULL;
  /* Will store the result of the RequestName RPC here. */
  guint result;
  GError* error = NULL;

  /*... Listing cut for brevity ...*/

  /* Initialize the GType/GObject system. */
  g_type_init();


  /* Create a main loop that will dispatch callbacks. */
  mainloop = g_main_loop_new(NULL, FALSE);

  /*... Listing cut for brevity ...*/

  g_print("main Connecting to the Session D-Bus.\n");
  bus = dbus_g_bus_get(DBUS_BUS_SESSION, &error);
  if (error != NULL) {
    /* Print error and terminate. */
   // handleError("Couldn't connect to session bus", error->message, TRUE);
  }



  g_print("main Registering the well-known name (%s)\n",
          VALUE_SERVICE_NAME);

  /* In order to register a well-known name, we need to use the
     "RequestMethod" of the /org/freedesktop/DBus interface. Each
     bus provides an object that will implement this interface.

     In order to do the call, we need a proxy object first.
     DBUS_SERVICE_DBUS = "org.freedesktop.DBus"
     DBUS_PATH_DBUS = "/org/freedesktop/DBus"
     DBUS_INTERFACE_DBUS = "org.freedesktop.DBus" */
  busProxy = dbus_g_proxy_new_for_name(bus,
                                       DBUS_SERVICE_DBUS,
                                       DBUS_PATH_DBUS,
                                       DBUS_INTERFACE_DBUS);
  if (busProxy == NULL) {
    //handleError("Failed to get a proxy for D-Bus", "Unknown(dbus_g_proxy_new_for_name)", TRUE);
  }

  /* Attempt to register the well-known name.
     The RPC call requires two parameters:
     - arg0: (D-Bus STRING): name to request
     - arg1: (D-Bus UINT32): flags for registration.
       (please see "org.freedesktop.DBus.RequestName" in
        http://dbus.freedesktop.org/doc/dbus-specification.html)
     Will return one uint32 giving the result of the RPC call.
     We're interested in 1 (we're now the primary owner of the name)
     or 4 (we were already the owner of the name, however in this
     application it wouldn't make much sense).

     The function will return FALSE if it sets the GError. */
  if (!dbus_g_proxy_call(busProxy,
                         /* Method name to call. */
                         "RequestName",
                         /* Where to store the GError. */
                         &error,
                         /* Parameter type of argument 0. Note that
                            since we're dealing with GLib/D-Bus
                            wrappers, you will need to find a suitable
                            GType instead of using the "native" D-Bus
                            type codes. */
                         G_TYPE_STRING,
                         /* Data of argument 0. In our case, this is
                            the well-known name for our server
                            example ("org.maemo.Platdev_ex"). */
                         VALUE_SERVICE_NAME,
                         /* Parameter type of argument 1. */
                         G_TYPE_UINT,
                         /* Data of argument 0. This is the "flags"
                            argument of the "RequestName" method which
                            can be use to specify what the bus service
                            should do when the name already exists on
                            the bus. We'll go with defaults. */
                         0,
                         /* Input arguments are terminated with a
                            special GType marker. */
                         G_TYPE_INVALID,
                         /* Parameter type of return value 0.
                            For "RequestName" it is UINT32 so we pick
                            the GType that maps into UINT32 in the
                            wrappers. */
                         G_TYPE_UINT,
                         /* Data of return value 0. These will always
                            be pointers to the locations where the
                            proxy can copy the results. */
                         &result,
                         /* Terminate list of return values. */
                         G_TYPE_INVALID)) {
    //handleError("D-Bus.RequestName RPC failed", error->message,TRUE);
    /* Note that the whole call failed, not "just" the name
       registration (we deal with that below). This means that
       something bad probably has happened and there's not much we can
       do (hence program termination). */
  }
  /* Check the result code of the registration RPC. */
  g_print(":main RequestName returned %d.\n", result);
  if (result != 1) {
   // handleError("Failed to get the primary well-known name.", "RequestName result != 1", TRUE);
    /* In this case we could also continue instead of terminating.
       We could retry the RPC later. Or "lurk" on the bus waiting for
       someone to tell us what to do. If we would be publishing
       multiple services and/or interfaces, it even might make sense
       to continue with the rest anyway.

       In our simple program, we terminate. Not much left to do for
       this poor program if the clients won't be able to find the
       Value object using the well-known name. */
  }

    g_print(":main Creating one Value object.\n");
  /* The NULL at the end means that we have stopped listing the
     property names and their values that would have been used to
     set the properties to initial values. Our simple Value
     implementation does not support GObject properties, and also
     doesn't inherit anything interesting from GObject directly, so
     there are no properties to set. For more examples on properties
     see the first GTK+ example programs from the maemo Application
     Development material.

     NOTE: You need to keep at least one reference to the published
           object at all times, unless you want it to disappear from
           the D-Bus (implied by API reference for
           dbus_g_connection_register_g_object(). */
  valueObj = g_object_new(VALUE_TYPE_OBJECT, NULL);
  if (valueObj == NULL) {
   // handleError("Failed to create one Value instance.", "Unknown(OOM?)", TRUE);
  }

  g_print(":main Registering it on the D-Bus.\n");
  /* The function does not return any status, so can't check for
     errors here. */
  dbus_g_connection_register_g_object(bus,
                                      VALUE_SERVICE_OBJECT_PATH,
                                      G_OBJECT(valueObj));

  g_print( ":main Ready to serve requests (daemonizing).\n");

  /*... Listing cut for brevity ...*/

   /* Start service requests on the D-Bus forever. */
  g_main_loop_run(mainloop);
  /* This program will not terminate unless there is a critical
     error which will cause abort() to be used. Hence it will never
     reach this point. */

  /* If it does, return failure exit code just in case. */
  return EXIT_FAILURE;

}
