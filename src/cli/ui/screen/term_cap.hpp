#pragma once

namespace ccl::cli::ui
{
    struct TCapabilities
    {
        constexpr static const char* RESET            = "sgr0";             // Reset Attributes
        constexpr static const char* START_UNDERLINE  = "smul";             // Enter underline mode
        constexpr static const char* STOP_UNDERLINE   = "rmul";             // Exit underline mode
        constexpr static const char* BLINK_TEXT       = "blink";            // Enable blinking text
        constexpr static const char* FOREGROUND_C     = "setaf";            // Set foreground color
        constexpr static const char* BACKGROUND_C     = "setab";            // Set background color
        constexpr static const char* BOLD_TEXT        = "bold";             // Enable bold text
        constexpr static const char* REVERSE_VIDEO    = "rev";              // Swap frgnd and bkgnd color
        constexpr static const char* RGB_FOREGROUND_C = "set_a_foreground"; // Set foreground color (truecolor)
        constexpr static const char* RGB_BACKGROUND_C = "set_a_background"; // Set background color (truecolor)
        constexpr static const char* MOVE_CURSOR      = "cup";              // Move cursor to position
        constexpr static const char* CLEAR_SCREEN     = "clear";            // Clear the entire screen
        constexpr static const char* SAVE_CURSOR      = "sc";               // Save the cursor position
        constexpr static const char* RESTORE_CURSOR   = "rc";               // Restore the cursor position
        constexpr static const char* HIDE_CURSOR      = "civis";            // Hide the cursor
        constexpr static const char* SHOW_CURSOR      = "cnorm";            // Show the cursor
        constexpr static const char* KEY_MOUSE        = "key_mouse";        // Terminal support mouse input
        constexpr static const char* MOUSE_PROTO      = "kmous";            // Mouse protocol
        constexpr static const char* ENTER_ALT_SCREEN = "smcup";            // Enter alternate screen
        constexpr static const char* EXIT_ALT_SCREEN  = "rmcup";            // Exit alternate screen
    };
}