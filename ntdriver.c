#include "stdafx.h"
#include "$safeprojectname$.h"

VOID $safeprojectname$Unload(IN PDRIVER_OBJECT DriverObject);
NTSTATUS $safeprojectname$CreateClose(IN PDEVICE_OBJECT DeviceObject, IN PIRP Irp);
NTSTATUS $safeprojectname$DefaultHandler(IN PDEVICE_OBJECT DeviceObject, IN PIRP Irp);
NTSTATUS DriverEntry(IN PDRIVER_OBJECT DriverObject,
                     IN PUNICODE_STRING RegistryPath);

#pragma alloc_text(PAGE, $safeprojectname$Unload)
#pragma alloc_text(PAGE, $safeprojectname$CreateClose)
#pragma alloc_text(PAGE, $safeprojectname$DefaultHandler)

VOID $safeprojectname$Unload(IN PDRIVER_OBJECT DriverObject) {
  UNICODE_STRING Win32Device;

  RtlInitUnicodeString(&Win32Device, SYMBOL_NAME);
  IoDeleteSymbolicLink(&Win32Device);
  IoDeleteDevice(DriverObject->DeviceObject);
}

NTSTATUS $safeprojectname$CreateClose(IN PDEVICE_OBJECT DeviceObject, IN PIRP Irp) {
  Irp->IoStatus.Status = STATUS_SUCCESS;
  Irp->IoStatus.Information = 0;
  IoCompleteRequest(Irp, IO_NO_INCREMENT);
  return STATUS_SUCCESS;
}

NTSTATUS $safeprojectname$DefaultHandler(IN PDEVICE_OBJECT DeviceObject, IN PIRP Irp) {
  Irp->IoStatus.Status = STATUS_NOT_SUPPORTED;
  Irp->IoStatus.Information = 0;
  IoCompleteRequest(Irp, IO_NO_INCREMENT);
  return Irp->IoStatus.Status;
}

NTSTATUS DriverEntry(IN PDRIVER_OBJECT DriverObject,
                     IN PUNICODE_STRING RegistryPath) {
  UNICODE_STRING DeviceName, Win32Device;
  PDEVICE_OBJECT DeviceObject = NULL;
  NTSTATUS Status = STATUS_UNSUCCESSFUL;
  unsigned idx = 0;

  RtlInitUnicodeString(&DeviceName, DEVICE_NAME);
  RtlInitUnicodeString(&Win32Device, SYMBOL_NAME);

  for (idx = 0; idx <= IRP_MJ_MAXIMUM_FUNCTION; idx++)
    DriverObject->MajorFunction[idx] = $safeprojectname$DefaultHandler;

  DriverObject->MajorFunction[IRP_MJ_CREATE] = $safeprojectname$CreateClose;
  DriverObject->MajorFunction[IRP_MJ_CLOSE] = $safeprojectname$CreateClose;

  DriverObject->DriverUnload = $safeprojectname$Unload;
  Status = IoCreateDevice(DriverObject, 0, &DeviceName, FILE_DEVICE_UNKNOWN, 0,
                          FALSE, &DeviceObject);
  if (!NT_SUCCESS(Status)) {
    return Status;
  }
  if (!DeviceObject) {
    return STATUS_UNEXPECTED_IO_ERROR;
  }

  DeviceObject->Flags |= DO_DIRECT_IO;
  DeviceObject->AlignmentRequirement = FILE_WORD_ALIGNMENT;
  Status = IoCreateSymbolicLink(&Win32Device, &DeviceName);

  DeviceObject->Flags &= ~DO_DEVICE_INITIALIZING;

  return STATUS_SUCCESS;
}
