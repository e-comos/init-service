#!/bin/bash

echo "=== E-comOS Boot Animation Tests ==="
echo

echo "1. Dots Theme Test"
echo "   - Multi-row animated dots"
echo "   - No dependencies"
echo
read -p "Run dots test? (y/n): " -n 1 -r
echo
if [[ $REPLY =~ ^[Yy]$ ]]; then
    gcc test_pure.c -o test_pure
    ./test_pure
fi

echo
echo "2. Whistler Theme Test"
echo "   - Classic color bar animation"
echo
read -p "Run Whistler test? (y/n): " -n 1 -r
echo
if [[ $REPLY =~ ^[Yy]$ ]]; then
    gcc whistler_test.c -o whistler_test
    ./whistler_test
fi

echo
echo "3. Integration with Your Kernel"
echo "   Link these files with your kernel:"
echo "   - init.o"
echo "   - boot_animation.o"
echo "   - init_entry.o"
echo
echo "   Then call: init_service_entry()"
echo
echo "Tests complete!"