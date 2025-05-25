#!/bin/bash

echo -e "\n📱 ATM Management System Setup Script 📱\n"

# Check if data directory exists; if not, create them; else bypass
if [ ! -d "data" ] || [ ! -f "data/users.txt" ] || [ ! -f "data/records.txt" ] || [ ! -f "data/notifications.txt" ] || [ ! -f "data/temp_notif.txt" ]; then
    echo "🔄 Setting up required directories and files..."
    make setup
    echo "✅ Setup completed successfully!"
else
    echo "✅ Data directory and files already exist!"
fi

# Display help message
echo -e "\n📌 Available Commands:"
echo "━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━"
echo "  make        → Build the program"
echo "  make run    → Build and run the program"
echo "  make clean  → Remove compiled files"
echo "  make rebuild→ Clean and rebuild everything"
echo "━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━"

echo -e "\n🚀 Quick Start:"
echo "1. Run 'make' to build the program"
echo "2. Run 'make run' to start the ATM system"
echo -e "\n❓ For help, refer to the README.md file\n" 