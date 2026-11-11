#pragma once

#include <esp_http_server.h>

class AethrOTA {
  public:
    // MAINT only. Starts the server and registers /update.
    // Pass a config to override the defaults, or nothing to use them.
    esp_err_t setup(const httpd_config_t* config = nullptr);

    httpd_handle_t handle() const {
        return server;
    } // for the Svelte page later

    void validate(bool healthy); // every mode

  private:
    httpd_handle_t server = nullptr;
    bool validated        = false;
};
extern AethrOTA aethr;
