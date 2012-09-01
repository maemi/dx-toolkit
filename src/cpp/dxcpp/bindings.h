/** \file
 *
 * \brief Data objects and related functions.
 */

#ifndef DXCPP_BINDINGS_H
#define DXCPP_BINDINGS_H

#include <string>
#include <limits>
#include "dxcpp.h"

extern std::string g_WORKSPACE_ID;

//! The abstract base class for all data object remote handlers.

/**
 * DXDataObject contains the common methods to all handlers, such as creation, describing, setting
 * of properties, tags, types, visibility, details, etc.
 *
 * Each DXDataObject instance has two IDs associated with it: the ID of the data object ID it is
 * associated with, and a project ID to indicate which project's copy of the data the handler will
 * use. Note that it is possible to have two handlers with the same data object ID but different
 * project IDs. Both will access the same underlying data, but the name, properties, and tags are
 * local to the project and can be modified independently.
 */
class DXDataObject {
 protected:
  std::string dxid_;
  std::string proj_;
  void waitOnState(const std::string &state="closed",
		   const int timeout=std::numeric_limits<int>::max()) const;

  virtual dx::JSON describe_(const std::string &input_params) const = 0;
  virtual void addTypes_(const std::string &input_params) const = 0;
  virtual void removeTypes_(const std::string &input_params) const = 0;
  virtual dx::JSON getDetails_(const std::string &input_params) const = 0;
  virtual void setDetails_(const std::string &input_params) const = 0;
  virtual void setVisibility_(const std::string &input_params) const = 0;
  virtual void rename_(const std::string &input_params) const = 0;
  virtual void setProperties_(const std::string &input_params) const = 0;
  virtual void addTags_(const std::string &input_params) const = 0;
  virtual void removeTags_(const std::string &input_params) const = 0;
  virtual void close_(const std::string &input_params) const = 0;
  virtual dx::JSON listProjects_(const std::string &input_params) const = 0;

  /**
   * Clones the object into the specified project and folder.
   *
   * See the <a
   * href="http://wiki.dev.dnanexus.com/API-Specification-v1.0.0/Cloning#API-method%3A-%2Fclass-xxxx%2Fclone">/class-xxxx/clone</a>
   * API method for more info.
   *
   * @param dest_proj_id ID of the project into which the object should be cloned
   * @param dest_folder Folder route in which to put it in the destination project.
   */
  void clone_(const std::string &dest_proj_id,
              const std::string &dest_folder) const;

 public:
  DXDataObject() { }
  DXDataObject(const DXDataObject &to_copy) {
    dxid_ = to_copy.dxid_;
    proj_ = to_copy.proj_;
  }
  DXDataObject(const std::string &dxid) { setIDs(dxid); }
  /**
   * Creates a handler for the specified remote data object.
   *
   * @param dxid ID of the object.
   * @param proj ID of of the project via which to access the object.
   */
  DXDataObject(const std::string &dxid, const std::string &proj) {
    setIDs(dxid, proj);
  }

  /**
   * @return ID of the associated object.
   */
  std::string getID() const { return dxid_; }

  /**
   * This default conversion to string returns the object ID so a handler can always be passed
   * in place of a string argument that expects an object ID.
   */
  operator std::string() { return dxid_; }

  /**
   * @return ID of the project being used to access the data object.
   */
  std::string getProjectID() const { return proj_; }

  /**
   * Sets the object and project IDs as specified. If not specified, the project ID will be set
   * to the default workspace.
   *
   * See setWorkspaceID() and loadFromEnvironment() for more information.
   *
   * @param dxid ID of the associated object.
   * @param proj ID of the project whose copy of the data object should be accessed.
   */
  virtual void setIDs(const std::string &dxid,
		      const std::string &proj=g_WORKSPACE_ID);

  /**
   * Returns a JSON object with, at minimum, the keys "id", "class", "types", and "createdAt".
   * Other fields may also be included, depending on the class.
   *
   * @param incl_properties If true, properties are included in the output.
   * @param incl_details If true, details are included in the output.
   *
   * @return JSON hash containing description
   */
  dx::JSON describe(bool incl_properties=false, bool incl_details=false) const;

  /**
   * Adds the specified types to the object.
   *
   * See the <a
   * href="http://wiki.dev.dnanexus.com/API-Specification-v1.0.0/Types#API-method%3A-%2Fclass-xxxx%2FaddTypes">/class-xxxx/addTypes</a>
   * API method for more info.
   *
   * @param types JSON array of strings to add as types
   */
  void addTypes(const dx::JSON &types) const;

  /**
   * Removes the specified types from the object.
   *
   * See the <a
   * href="http://wiki.dev.dnanexus.com/API-Specification-v1.0.0/Types#API-method%3A-%2Fclass-xxxx%2FremoveTypes">/class-xxxx/removeTypes</a>
   * API method for more info.
   *
   * @param types JSON array of strings to remove as types
   */
  void removeTypes(const dx::JSON &types) const;

  /**
   * Retrieves the details stored in the object.
   *
   * See the <a
   * href="http://wiki.dev.dnanexus.com/API-Specification-v1.0.0/Details%20and%20Links#API-method%3A-%2Fclass-xxxx%2FgetDetails">/class-xxxx/getDetails</a>
   * API method for more info.
   *
   * @return JSON hash or array containing the remote object's details
   */
  dx::JSON getDetails() const;

  /**
   * Stores the given JSON in the details of the remote object.
   *
   * See the <a
   * href="http://wiki.dev.dnanexus.com/API-Specification-v1.0.0/Details%20and%20Links#API-method%3A-%2Fclass-xxxx%2FsetDetails">/class-xxxx/setDetails</a>
   * API method for more info.
   *
   * @param details Arbitrary JSON hash or array to store as details
   */
  void setDetails(const dx::JSON &details) const;

  /**
   * Ensures that the remote object is hidden.
   *
   * See the <a
   * href="http://wiki.dev.dnanexus.com/API-Specification-v1.0.0/Visibility#API-method%3A-%2Fclass-xxxx%2FsetVisibility">/class-xxxx/setVisibility</a>
   * API method for more info.
   */
  void hide() const;

  /**
   * Ensures that the remote object is visible.
   *
   * See the <a
   * href="http://wiki.dev.dnanexus.com/API-Specification-v1.0.0/Visibility#API-method%3A-%2Fclass-xxxx%2FsetVisibility">/class-xxxx/setVisibility</a>
   * API method for more info.
   */
  void unhide() const;

  /**
   * Renames the object.
   *
   * See the <a
   * href="http://wiki.dev.dnanexus.com/API-Specification-v1.0.0/Name#API-method%3A-%2Fclass-xxxx%2Frename">/class-xxxx/rename</a>
   * API method for more info.
   *
   * @param name New name for the object.
   */
  void rename(const std::string &name) const;

  /**
   * Sets the specified properties.
   *
   * See the <a
   * href="http://wiki.dev.dnanexus.com/API-Specification-v1.0.0/Properties#API-method%3A-%2Fclass-xxxx%2FsetProperties">/class-xxxx/setProperties</a>
   * API method for more info.
   *
   * @param properties JSON hash mapping strings to strings, containing names and values of
   * properties to set.
   */
  void setProperties(const dx::JSON &properties) const;

  /**
   * Retrieves all properties of the object.
   *
   * @return JSON hash mapping strings to strings, containing property names and their associated
   * values.
   */
  dx::JSON getProperties() const;

  /**
   * Adds the specified tags to the object.
   *
   * See the <a
   * href="http://wiki.dev.dnanexus.com/API-Specification-v1.0.0/Tags#API-method%3A-%2Fclass-xxxx%2FaddTags">/class-xxxx/addTags</a>
   * API method for more info.
   *
   * @param tags JSON array of strings to add as tags.
   */
  void addTags(const dx::JSON &tags) const;

  /**
   * Removes the specified tags from the object.
   *
   * See the <a
   * href="http://wiki.dev.dnanexus.com/API-Specification-v1.0.0/Tags#API-method%3A-%2Fclass-xxxx%2FremoveTags">/class-xxxx/removeTags</a>
   * API method for more info.
   *
   * @param tags JSON array of strings to remove as tags.
   */
  void removeTags(const dx::JSON &tags) const;

  /**
   * Closes the object.
   */
  virtual void close() const;

  /**
   * Lists all projects that contain a copy of the object.
   *
   * @return JSON array of project IDs (strings) that contain a copy of the object.
   */
  dx::JSON listProjects() const;

  /**
   * Moves the object into the specified folder in the same project.
   *
   * @param dest_folder Folder route in which to put the object
   */
  void move(const std::string &dest_folder) const;

  /**
   * Removes the copy of the object from the associated project (see getProjectID()). Copies of the
   * object in other projects are not affected.
   */
  void remove();
};

/**
 * Creates a JSON object that is a special DNAnexus link to an
 * existing data object ID.
 *
 * @param dxid Data object ID to link to.
 * @param proj Project ID to specify in the link.
 */
dx::JSON DXLink(const std::string &dxid, const std::string &proj="");

#include "bindings/dxrecord.h"
//#include "bindings/dxtable.h"
#include "bindings/dxfile.h"
#include "bindings/dxgtable.h"
#include "bindings/dxjob.h"
#include "bindings/dxapplet.h"
#include "bindings/dxapp.h"
#include "bindings/dxproject.h"
#include "bindings/search.h"

#endif
