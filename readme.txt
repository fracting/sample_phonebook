Query a prototype of a function by writting an invalid call of the function.
Example:
 - Code:
`CreateWindowExW();`
 - Result:
```
dummy.cpp: In function ‘int main()’:
dummy.cpp:7:21: error: too few arguments to function ‘HWND__* CreateWindowExW(DWORD, LPCWSTR, LPCWSTR, DWORD, int, int, int, int, HWND, HMENU, HINSTANCE, LPVOID)’
     CreateWindowExW();
                     ^
In file included from /usr/share/mingw-w64/include/windows.h:72:0,
                 from dummy.cpp:1:
/usr/share/mingw-w64/include/winuser.h:1935:26: note: declared here
   WINUSERAPI HWND WINAPI CreateWindowExW(DWORD dwExStyle,LPCWSTR lpClassName,LPCWSTR lpWindowName,DWORD dwStyle,int X,int Y,int nWidth,int nHeight,HWND hWndParent,HMENU hMenu,HINSTANCE hInstance,LPVOID lpParam);
```

Query a typedef of a type by writting an invalid typedef of the type.
Example:
 - Code:
`typedef void LPCWSTR;`
 - Result:
```
dummy.cpp:3:14: error: conflicting declaration ‘typedef void LPCWSTR’
 typedef void LPCWSTR;
              ^
In file included from /usr/share/mingw-w64/include/minwindef.h:146:0,
                 from /usr/share/mingw-w64/include/windef.h:8,
                 from /usr/share/mingw-w64/include/windows.h:69,
                 from dummy.cpp:1:
/usr/share/mingw-w64/include/winnt.h:278:24: note: previous declaration as ‘typedef const WCHAR* LPCWSTR’
   typedef CONST WCHAR *LPCWSTR,*PCWSTR;
                        ^
```

Make a pull request in order to trigger AppVeyor CI to start a rebuild.
