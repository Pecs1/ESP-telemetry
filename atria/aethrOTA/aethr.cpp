#include "aethr.h"

#include "atria/utils/logger.h"

#include <esp_ota_ops.h>

// docs:
// https://docs.espressif.com/projects/esp-idf/en/v5.5.6/esp32/api-reference/system/ota.html
// https://docs.espressif.com/projects/esp-idf/en/v5.5.6/esp32/api-reference/protocols/esp_http_server.html

#define MODULE_NAME "aethr"

namespace {
    esp_err_t updateHandler(httpd_req_t* req) {}
    esp_err_t indexHandler(httpd_req_t* req) {}

    // clang-format off
    static const httpd_uri_t indexGET = {
        .uri                     = "/",
        .method                  = HTTP_GET,
        .handler                 = indexHandler,
        .user_ctx                = NULL,
        .is_websocket            = true,
        .handle_ws_control_frames = false,
        .supported_subprotocol   = ""
    };

    static const httpd_uri_t updatePOST = {
        .uri                     = "/update",
        .method                  = HTTP_POST,
        .handler                 = updateHandler,
        .user_ctx                = NULL,
        .is_websocket            = false,
        .handle_ws_control_frames = false,
        .supported_subprotocol   = ""
    };
    // clang-format on

} // namespace

// *** protect functions ***
esp_err_t AethrOTA::setup(const httpd_config_t* config) {
    if (server != nullptr) {
        return ESP_ERR_INVALID_STATE; // already started
    }

    httpd_config_t cfg = HTTPD_DEFAULT_CONFIG();
    if (config != nullptr) {
        cfg = *config;
    } else {
        cfg.recv_wait_timeout = 15;
        cfg.stack_size        = 8192;
    }

    esp_err_t err = httpd_start(&server, &cfg);
    if (err != ESP_OK) {
        server = nullptr;
        return err;
    }

    err = httpd_register_uri_handler(server, &updatePOST);
    if (err == ESP_OK) {
        err = httpd_register_uri_handler(server, &indexGET);
    }

    if (err != ESP_OK) {
        httpd_stop(server);
        server = nullptr;
    }
    return err;
}

// void AethrOTA::verify() {}

// void AethrOTA::validate() {}

// // *** protected functions ***
// void AethrOTA::protectedSetup() {
//     logger.debug(MODULE_NAME, "starting...");
// }

// void AethrOTA::protectedVerify() {}

// void AethrOTA::protectedValidate() {}
AethrOTA aethr;
