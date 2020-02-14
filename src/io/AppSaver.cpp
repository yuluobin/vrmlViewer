//------------------------------------------------------------------------
//  Copyright (C) Gabriel Taubin
//  Time-stamp: <2019-11-03 14:54:11 taubin>
//------------------------------------------------------------------------
//
// AppSaver.cpp
//
// Software developed for the Fall 2019 Brown University course
// ENGN 2912B Scientific Computing in C++
// Copyright (c) 2019, Gabriel Taubin
// All rights reserved.
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted provided that the following conditions are met:
//     * Redistributions of source code must retain the above copyright
//       notice, this list of conditions and the following disclaimer.
//     * Redistributions in binary form must reproduce the above copyright
//       notice, this list of conditions and the following disclaimer in the
//       documentation and/or other materials provided with the distribution.
//     * Neither the name of the Brown University nor the
//       names of its contributors may be used to endorse or promote products
//       derived from this software without specific prior written permission.
//
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
// ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
// WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
// DISCLAIMED. IN NO EVENT SHALL GABRIEL TAUBIN BE LIABLE FOR ANY
// DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
// (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
// LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
// ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
// (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
// SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

#include "AppSaver.hpp"

bool AppSaver::save(const char* filename, SceneGraph& wrl) {
    bool success = false;
    if (filename != (const char*)0) {
        // int n = (int)strlen(filename);
        int n = (int)string(filename).size();
        int i;
        for (i = n - 1; i >= 0; i--)
            if (filename[i] == '.')
                break;
        if (i >= 0) {
            string ext(filename + i + 1);
            Saver* saver = _registry[ext];
            if (saver != (Saver*)0)
                success = saver->save(filename, wrl);
        }
    }
    return success;
}

void AppSaver::registerSaver(Saver* saver) {
    if (saver != (Saver*)0) {
        string ext(saver->ext()); // constructed from const char*
        pair<string, Saver*> ext_saver(ext, saver);
        _registry.insert(ext_saver);
    }
}
