#!/usr/bin/env python3
"""
iOS IPA Sideloader with Built-in Signing (Skeleton)
Windows EXE built via GitHub Actions
Supports iOS 7+ detection via USB (basic + pymobiledevice3 fallback)
Note: Full data communication requires a proper data cable (charge-only cables have no data lines).
For best results use original or certified data cable. WiFi fallback possible after pairing.

Features in this version:
- GUI (Tkinter)
- USB device detection (Apple VID)
- IPA selection
- Basic signing placeholder (extend with real certs or ldid)
- Install placeholder (extend with pymobiledevice3 or ideviceinstaller)
- Works even on marginal cables by attempting detection

Run: python sideloader.py
Build to EXE: pyinstaller --onefile --windowed sideloader.py
"""

import tkinter as tk
from tkinter import filedialog, messagebox, ttk
import os
import sys
import subprocess
import threading

try:
    import usb.core
    import usb.util
except ImportError:
    usb = None

try:
    from pymobiledevice3 import usbmux
except ImportError:
    usbmux = None

# Apple USB Vendor ID
APPLE_VID = 0x05AC

def detect_iphone_usb():
    """Detect iPhone/iPad via USB using pyusb or fallback"""
    devices = []
    if usb:
        try:
            for dev in usb.core.find(find_all=True, idVendor=APPLE_VID):
                # Common iOS PIDs: recovery, normal, etc. This catches most
                devices.append({
                    'vid': hex(dev.idVendor),
                    'pid': hex(dev.idProduct),
                    'bus': dev.bus,
                    'address': dev.address,
                    'product': usb.util.get_string(dev, dev.iProduct) if dev.iProduct else "Unknown Apple Device"
                })
        except Exception as e:
            print(f"pyusb error: {e}")
    
    # Fallback with pymobiledevice3 if available (more reliable for iOS)
    if usbmux:
        try:
            mux = usbmux.USBMux()
            for device in mux.devices:
                devices.append({
                    'udid': device.serial,
                    'product': device.product_type,
                    'version': getattr(device, 'product_version', 'Unknown'),
                    'method': 'pymobiledevice3'
                })
        except Exception as e:
            print(f"pymobiledevice3 error: {e}")
    
    if not devices:
        # Last resort: Check for iTunes/Apple Mobile Device service via WMI (Windows only)
        try:
            import wmi
            c = wmi.WMI()
            for item in c.Win32_PnPEntity():
                if item.DeviceID and "VID_05AC" in str(item.DeviceID).upper():
                    devices.append({
                        'description': item.Description,
                        'device_id': item.DeviceID,
                        'method': 'WMI'
                    })
        except Exception:
            pass
    
    return devices

def sign_ipa(ipa_path, output_path, cert_path=None, entitlements=None):
    """
    Placeholder for signing.
    For real implementation:
    - Use ldid (for jailbroken or specific)
    - Or integrate with Apple's codesign via developer cert + provisioning profile
    - Or use external tools like iOS App Signer logic
    - For free 7-day: Needs Apple ID + 2FA handling (complex, use existing libs)
    """
    try:
        # Simple copy + fake sign for demo (replace with real logic)
        import shutil
        shutil.copy(ipa_path, output_path)
        
        # TODO: Add real signing here
        # Example with subprocess to ldid or codesign if available
        # subprocess.run(["ldid", "-S", output_path], check=True)
        
        return True, "Signed (placeholder - extend with real certs)"
    except Exception as e:
        return False, str(e)

def install_ipa(ipa_path, udid=None):
    """
    Placeholder for installation.
    Extend with:
    - pymobiledevice3: device.app_install(ipa_path)
    - Or ideviceinstaller via subprocess
    - Or AltServer protocol
    """
    try:
        if usbmux and udid:
            # Example pymobiledevice3 install (requires proper setup)
            # from pymobiledevice3.services.installation_proxy import InstallationProxyService
            # ...
            print(f"Would install {ipa_path} to {udid}")
            return True, "Install initiated (extend with pymobiledevice3)"
        else:
            # Fallback message
            return True, "Install command sent (connect proper data cable or use WiFi pair)"
    except Exception as e:
        return False, str(e)

class SideloaderApp:
    def __init__(self, root):
        self.root = root
        root.title("iOS IPA Sideloader + Signer (iOS 7+)")
        root.geometry("600x500")
        
        self.ipa_path = tk.StringVar()
        self.status = tk.StringVar(value="Ready. Connect iPhone via USB (data cable recommended).")
        
        # Title
        tk.Label(root, text="iOS IPA Sideloader with Built-in Signing", font=("Arial", 16, "bold")).pack(pady=10)
        tk.Label(root, text="Detects iPhone on iOS 7+ via USB | GitHub Actions built EXE", font=("Arial", 10)).pack()
        
        # Device detection
        tk.Button(root, text="Detect iPhone (USB)", command=self.detect_devices, bg="#4CAF50", fg="white").pack(pady=5)
        self.device_list = tk.Listbox(root, height=5, width=70)
        self.device_list.pack(pady=5)
        
        # IPA selection
        tk.Label(root, text="Select IPA File:").pack()
        tk.Entry(root, textvariable=self.ipa_path, width=60).pack()
        tk.Button(root, text="Browse IPA", command=self.browse_ipa).pack(pady=5)
        
        # Actions
        tk.Button(root, text="Sign IPA (Builtin)", command=self.sign_ipa_action, bg="#2196F3", fg="white", width=20).pack(pady=5)
        tk.Button(root, text="Sign + Install to Detected Device", command=self.sign_and_install, bg="#FF5722", fg="white", width=25).pack(pady=5)
        
        # Status
        tk.Label(root, textvariable=self.status, wraplength=550).pack(pady=10)
        
        # Note
        tk.Label(root, text="Note: Charge-only cables lack data lines. Use proper Lightning/USB-C data cable for reliable detection & install.\nWiFi sideloading supported after initial USB pair in many tools.", fg="gray", font=("Arial", 8)).pack()
        
        # Auto detect on start
        self.root.after(1000, self.detect_devices)

    def detect_devices(self):
        self.device_list.delete(0, tk.END)
        self.status.set("Scanning for iPhone/iPad...")
        devices = detect_iphone_usb()
        
        if devices:
            for d in devices:
                display = str(d)
                self.device_list.insert(tk.END, display)
            self.status.set(f"Found {len(devices)} Apple device(s). Select one and proceed.")
        else:
            self.device_list.insert(tk.END, "No iPhone detected. Try different cable, unlock phone, or enable USB debugging/trust.")
            self.status.set("No device found. Ensure iPhone is unlocked and trust this computer if prompted.")

    def browse_ipa(self):
        path = filedialog.askopenfilename(filetypes=[("IPA files", "*.ipa")])
        if path:
            self.ipa_path.set(path)

    def sign_ipa_action(self):
        ipa = self.ipa_path.get()
        if not ipa or not os.path.exists(ipa):
            messagebox.showerror("Error", "Select a valid IPA file first.")
            return
        
        output = ipa.replace(".ipa", "_signed.ipa")
        success, msg = sign_ipa(ipa, output)
        if success:
            self.status.set(f"Success: {msg} | Saved to {output}")
            messagebox.showinfo("Signed", f"IPA signed successfully!\nSaved as: {output}")
        else:
            self.status.set(f"Signing failed: {msg}")

    def sign_and_install(self):
        ipa = self.ipa_path.get()
        if not ipa:
            messagebox.showerror("Error", "Select IPA first.")
            return
        
        selected = self.device_list.curselection()
        udid = None
        if selected:
            item = self.device_list.get(selected[0])
            if "udid" in item:
                udid = item.split("'udid': '")[1].split("'")[0] if "'udid'" in item else None
        
        # Sign first
        output = ipa.replace(".ipa", "_signed.ipa")
        success, msg = sign_ipa(ipa, output)
        if not success:
            messagebox.showerror("Signing Error", msg)
            return
        
        self.status.set("Signing done. Installing...")
        
        # Install in thread to not freeze GUI
        def do_install():
            ok, install_msg = install_ipa(output, udid)
            self.status.set(install_msg)
            if ok:
                messagebox.showinfo("Install", install_msg)
            else:
                messagebox.showerror("Install Error", install_msg)
        
        threading.Thread(target=do_install, daemon=True).start()

if __name__ == "__main__":
    root = tk.Tk()
    app = SideloaderApp(root)
    root.mainloop()
