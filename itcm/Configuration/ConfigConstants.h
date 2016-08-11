#ifndef ITCM_CONFIG_CONSTANTS_H
#define ITCM_CONFIG_CONSTANTS_H

#include <string>
#include <itcm/common/Constants.h>

/**
 * Contains the keys for accessing configuration file values.  Contains constants used for comparisons.
 */
namespace ITCM
{
namespace ConfigConstants
{

enum ConnectionType
{
	TYPE_APPLICATION_CONNECTION = 0,
	TYPE_ITCM_SYSTEM_CONNECTION
};

const std::string APP_CONNECTION_SUFFIX = "App";
const std::string ITCM_CONNECTION_SUFFIX = "ITCM";

const std::string AG_COMPONENT_TYPE = "AG";

/**
 * These constants are used for comparison against the AG's config file.
 */

const char SEPARATOR = '.';

const std::string PAYLOAD_EMP = "EMP";
const std::string PAYLOAD_ATCS = "ATCS";

const std::string AMQP = "AMQP";
const std::string CLASS_D = "ClassD";

const std::string CLASSD_ROLE_SERVER = "server";
const std::string CLASSD_ROLE_CLIENT = "client";

const std::string DIRECTION_TO_APP = "ToApp";
const std::string DIRECTION_FROM_APP = "FromApp";

const size_t MAX_BODY_LENGTH = 16777216; // 16MB
const size_t SERVICE_REQUEST_BITS = 3;
const size_t NETWORK_PREFERENCE_BITS = 3;
const size_t DATA_LENGTH_BYTES = 3;

}  //END ConfigConstants namespace

} //END ITCM namespace
#endif /* ITCM_CONFIG_CONSTANTS_H */
