/**
 * Init-Service , a init service for E-comOS
 * Copyright (C) 2025,2026 Saladin5101
 * 
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Affero General Public License as published
 * by the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 * 
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Affero General Public License for more details.
 * 
 * You should have received a copy of the GNU Affero General Public License
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
 */
// Entry point for init service when called from kernel
// This provides a clean interface between kernel and init service
extern int main(void); // From init.c

// Kernel calls this function to start init service
void init_service_entry(void) {
    main();
}

// Alternative: if kernel needs specific initialization
void init_service_start(void* kernel_info) {
    // Optional: process kernel_info if needed
    main();
}