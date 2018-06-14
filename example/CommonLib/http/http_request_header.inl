
//-------------------------------------------------------------------------------------
/**
    HTTP request header info.
*/
struct HTTP_REQUEST_HEADER
{
private:
    std::string   m_post_data ;

public:
    enum VERB_TYPE
    {
        VERB_TYPE_GET = 0,
        VERB_TYPE_POST = 1,
        VERB_TYPE_POST_MULTIPART = 2,
        VERB_TYPE_DELETE = 3,
    };

    /**
        verb of request, can be :\n\n
        VERB_TYPE_GET\n
        VERB_TYPE_POST\n
        VERB_TYPE_POST_MULTIPART\n
        VERB_TYPE_DELETE\n
    */
    int m_tag;
    const void* m_user_data;
    VERB_TYPE m_verb ;
    std::string   m_url ; ///< url to send request
    int       m_start ; ///< start position of get, in byte, default is 0, a range field will be added to HTTP request header if it is not 0
    std::string   m_user_agent; ///< default is same to IE
    /**
        header field, \\r\\n at end of each line, default is empty, don't include Range field if you have set m_start member\n\n
        m_header += _T(\"Accept-Encoding: gzip, deflate\\r\\n\") ;\n
        m_header += _T(\"Accept: text/html\\r\\n\") ;\n
    */
    std::string   m_header;
    /// param dwFlags of Win32API <b>HttpOpenRequest</b>, default is <b>INTERNET_FLAG_RELOAD | INTERNET_FLAG_NO_CACHE_WRITE</b>
    DWORD     m_open_flag ;
    /**
        request lifetime limit, in milliseconds, FCHttpRequestManager::OnOverRequestLifetime will be called when request over its lifetime\n
        default is 0, meaning no lifetime limit.
    */
    int       m_lifetime_limit ;

    std::string   m_proxy_ip; ///< proxy server IP
    int       m_proxy_port ; ///< proxy server port, default is 80
    std::string   m_proxy_username; ///< user name to login the proxy if need
    std::string   m_proxy_password; ///< password to login the proxy if need

    /// Default to construct a HTTP get request.
    HTTP_REQUEST_HEADER (VERB_TYPE nVerb=VERB_TYPE_GET)
    {
        m_verb = nVerb ;
        m_start = 0 ;
        m_user_agent = GetSystemUserAgent() ;
        m_open_flag = INTERNET_FLAG_RELOAD | INTERNET_FLAG_NO_CACHE_WRITE ;
        m_lifetime_limit = 0 ;
        m_proxy_port = 80 ;
        m_tag = -1;
        m_user_data = NULL;
    }

    /// Get text of current verb.
    std::string GetVerbText() const
    {
        switch (m_verb)
        {
            case VERB_TYPE_GET: return std::string("GET");
            case VERB_TYPE_POST :
            case VERB_TYPE_POST_MULTIPART: return std::string("POST");
            case VERB_TYPE_DELETE: return std::string("DELETE");
        }
        return std::string("GET");
    }

    static std::string StrFormat(const char * format, ...)
    {
        char buf[1024];
        va_list arglist;
        va_start(arglist, format);
        _vsnprintf_s(buf, 1024, format, arglist);
        va_end(arglist);
        return std::string(buf);
    }

    /// Get host and object from current url.
    void GetUrlComponent(std::string& sHost, std::string& sObject, int& port, INTERNET_SCHEME& nScheme) const
    {
        URL_COMPONENTSA   uc ;
        ZeroMemory (&uc, sizeof(uc)) ;

        DWORD   nBufferSize = m_url.size() + 8 ;

        std::vector<CHAR>   t1 (nBufferSize, (CHAR)0) ;
        std::vector<CHAR>   t2 (nBufferSize, (CHAR)0) ;

        uc.dwStructSize = sizeof(uc) ;
        uc.lpszHostName = &t1[0] ;
        uc.dwHostNameLength = nBufferSize ;
        uc.lpszUrlPath = &t2[0] ;
        uc.dwUrlPathLength = nBufferSize ;

        InternetCrackUrlA (m_url.c_str(), m_url.size(), 0, &uc) ;
        sHost = &t1[0] ;
        sObject = &t2[0] ;
        nScheme = uc.nScheme ;
        port = uc.nPort;
    }

    /// @name Add post data.
    //@{
    /// Add string to post data, <b>not</b> including terminator.
    void AddPostData(const std::string& s)
    {
        AddPostData ((LPCSTR)s.c_str(), s.size()) ;
    }
    /// Add buffer to post data.
    void AddPostData (const void* pBuffer, size_t nSize)
    {
        m_post_data.append ((const char*)pBuffer, nSize) ;
    }

    /// Set post data.
    void SetPostData(const std::string& s)
    {
        m_post_data = s;
    }
    //@}

    /// @name Add multipart/form-data.
    //@{
    /// Add string param, <b>not</b> including terminator.
    void AddMultipartFormData(const std::string& sName, const std::string& sValue)
    {
        AddMultipartFormData (sName, (LPCSTR)sValue.c_str(), sValue.size(), NULL) ;
    }
    /// Add buffer param.
    void AddMultipartFormData(const std::string& sName, const void* pValue, size_t nValueSize, LPCSTR sFileName)
    {
        std::string   s1 = "--" + DEFAULT_BOUNDARY() + "\r\n";

        s1 += "Content-Disposition: form-data; name=\"" + sName + "\"" ;
        if (sFileName)
        {
            s1 += "; filename=\"" + std::string(sFileName) + "\"";
        }
        s1 += "\r\n" ;

        s1 += "\r\n" ; // blank line
        AddPostData (s1) ;
        AddPostData (pValue, nValueSize) ;
        AddPostData ("\r\n") ; // after data
    }

    /// End add param.
    void EndMultipartFormData()
    {
        // terminator
        AddPostData ("--" + DEFAULT_BOUNDARY() + "--\r\n") ;

        m_header += "Accept: */*\r\n";
        m_header += std::string("Content-Type: multipart/form-data; boundary=") + std::string(DEFAULT_BOUNDARY()) + std::string("\r\n");
        m_header.append(StrFormat("Content-Length: %d\r\n", (int)m_post_data.size()));
    }
    //@}

private:
    // Get user-agent used by IE.
    static std::string GetSystemUserAgent()
    {
        DWORD  n = 1024 ;
        char   t[1024] = {0} ;
        ObtainUserAgentString (0, t, &n) ;
        return std::string(t);
    }

    static std::string DEFAULT_BOUNDARY()
    {
        // D514116929C24f35AE047F2B82C59B08 , avoid to find string in binary including this library
        return "---" + std::string("D514116929C24f35") + std::string("AE047F2B82C59B08");
    }

friend class FCHttpRequest ; // to access m_post_data
};
