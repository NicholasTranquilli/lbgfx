#pragma once

#define lb_SwapEndien(what) what = lb::EndienSwap(what)

namespace lb
{
    template <typename _T>
    _T EndienSwap(_T _data)
    {
        union byte_data {
            _T data;
            char bdata[sizeof(_T)];
        } in = { _data }, out;

        for (int i = 0; i <= sizeof(_T); i++)
            out.bdata[i] = in.bdata[sizeof(_T) - i - 1];

        return out.data;
    }

    template <typename _T>
    void EndienSwap_c(_T& _data)
    {
        union byte_data {
            _T data;
            char bdata[sizeof(_T)];
        } in = { _data }, out;

        for (int i = 0; i <= sizeof(_T); i++)
            out.bdata[i] = in.bdata[sizeof(_T) - i - 1];

        _data = out.data;
    }

    template <typename _T>
    _T& ByteToVal(unsigned char* p_bytes, int& r_nextByte, size_t size = sizeof(_T))
    {
        _T elem = 0;

        for (int i = 0; i < size; i++)
            elem |= p_bytes[i] << ((abs(i - ((int)size - 1))) * 8);

        r_nextByte = r_nextByte + size;

        return elem;
    }

    template <typename _T>
    _T& ByteToVal(unsigned char* p_bytes, size_t size = sizeof(_T))
    {
        int tmp;
        return lb::ByteToVal<_T>(p_bytes, tmp, size);
    }
}