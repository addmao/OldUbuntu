package practicum;

import java.util.Scanner;

import org.usb4java.Device;

public class TestApp {
    static McuBoard board;
    static Scanner scanner;
    
    static void chase(int delay) {
        board.write((byte)4, (short)0, (short)delay);
    }
    static int switchCount() {
        byte[] data = board.read((byte)5, (short)0, (short)0);
        return data[0];
    }
    static int getChoice() {
        System.out.println();
        System.out.println("Main Menu");
        System.out.println("1. Slow LED chase");
        System.out.println("2. Fast LED chase");
        System.out.println("3. Count switch presses");
        System.out.println("4. Quit");
        System.out.print("Enter your choice: ");
        int choice = scanner.nextInt();
        return choice;
    }
    public static void main(String[] args) {
        McuBoard.initUsb();
        Device[] devices = McuBoard.findBoards();
        
        if (devices.length == 0) {
            System.out.format("** Practicum board not found **\n");
            return;
        }
        else {
            System.out.format("** Found %d practicum board(s) **\n", devices.length);
        }
        board = new McuBoard(devices[0]);

        System.out.format("** Practicum board found **\n");
        System.out.format("** Manufacturer: %s\n", board.getManufacturer());
        System.out.format("** Product: %s\n", board.getProduct());

        scanner = new Scanner(System.in);
        for (;;) {
            int choice = getChoice();
            if (choice == 1)
                chase(500);
            else if (choice == 2)
                chase(100);
            else if (choice == 3)
                System.out.printf("Switch has been pressed %d time(s) since last read\n", switchCount());
            else if (choice == 4)
                break;
        }
    }
}
