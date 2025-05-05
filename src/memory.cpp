#include "memory.hpp"
#include "global.hpp"
#include "simple_button.hpp"
#include "utils/hex_string.hpp"

Memory::Memory(uint64_t address) {
  try {
    mDataView = new DataView(address);
    mReturnBtn = Button(
        "↵",
        [&] {
          try {
            if (mAddrInputStr.empty()) {
              *gMessageStr = "Please enter the address you want to move to.";
              *gShowMessageModal = true;
              return;
            }
            
            size_t pos;
            uint64_t newAddr = stoul(mAddrInputStr, &pos, 16);
            if (pos != mAddrInputStr.length()) {
              *gMessageStr = "Invalid hex value. Please enter a valid hex address.";
              *gShowMessageModal = true;
              return;
            }
            
            mDataView->setAddress(newAddr);
            mAddrInputStr = hexString(mDataView->getAddress(), 16);
          } catch (const invalid_argument&) {
            *gMessageStr = "The address must be a valid hex value.";
            *gShowMessageModal = true;
          } catch (const out_of_range&) {
            *gMessageStr = "The address value is too large.";
            *gShowMessageModal = true;
          } catch (const exception& e) {
            *gMessageStr = string("Error: ") + e.what();
            *gShowMessageModal = true;
          }
        },
        SimpleButton());
    mPreviousAddrBtn = Button(
        "◀",
        [&] {
          if (mDataView->getAddress() < 0x100) {
            *gMessageStr = "It is not possible to move from the current address "
                           "to the address before 0x100.";
            *gShowMessageModal = true;
          } else {
            mDataView->toPrevious();
            mAddrInputStr = hexString(mDataView->getAddress(), 16);
          }
        },
        SimpleButton());
    mNextAddrBtn = Button(
        "▶",
        [&] {
          mDataView->toNext();
          mAddrInputStr = hexString(mDataView->getAddress(), 16);
        },
        SimpleButton());
    mRefreshBtn = Button(
        "Refresh",
        [&] {
          mDataView->refresh();
          mAddrInputStr = hexString(mDataView->getAddress(), 16);
        },
        SimpleButton());
    mAddrInputStr = hexString(address, 16);
    mAddrInput = Input(&mAddrInputStr, "Address") | size(WIDTH, LESS_THAN, 17);
    mDataViewComponent = mDataView->getComponent();
    mContainer = Container::Vertical({
                     Container::Horizontal({
                         mAddrInput,
                         mReturnBtn,
                         mPreviousAddrBtn,
                         mNextAddrBtn,
                         mRefreshBtn,
                     }),
                     mDataViewComponent,
                 }) |
                 CatchEvent([&](Event event) {
                   if (event == Event::PageUp) {
                     if (mDataView->getAddress() < 0x100) {
                       *gMessageStr =
                           "It is not possible to move from the current address "
                           "to the address before 0x100.";
                       *gShowMessageModal = true;
                     } else {
                       mDataView->toPrevious();
                       mAddrInputStr = hexString(mDataView->getAddress(), 16);
                     }
                     return true;
                   }
                   if (event == Event::PageDown) {
                     mDataView->toNext();
                     mAddrInputStr = hexString(mDataView->getAddress(), 16);
                     return true;
                   }
                   if (event == Event::F2) {
                     mAddrInput->TakeFocus();
                     return true;
                   }
                   if (event == Event::F5) {
                     mDataView->refresh();
                     mAddrInputStr = hexString(mDataView->getAddress(), 16);
                     return true;
                   }
                   return false;
                 });
  } catch (const exception& e) {
    *gMessageStr = string("Failed to initialize memory view: ") + e.what();
    *gShowMessageModal = true;
    throw;
  }
}

Memory::~Memory() {
  delete mDataView;
}

Component Memory::getComponent() {
  return Renderer(mContainer, [&] {
    if (mAddrInputStr.length() > 16) {
      mAddrInputStr = mAddrInputStr.substr(0, 16);
    }
    return vbox({
        hbox({
            text(" * Address : "),
            text("0x"),
            mAddrInput->Render(),
            text(" "),
            mReturnBtn->Render(),
            text(" "),
            mPreviousAddrBtn->Render(),
            text(" "),
            mNextAddrBtn->Render(),
            text(" "),
            mRefreshBtn->Render(),
        }),
        hbox({
            mDataViewComponent->Render() | size(WIDTH, LESS_THAN, 78) |
                size(HEIGHT, EQUAL, 17) | border,
        }),
    });
  });
}
