# ImageBasedMapPacman

이미지에 기반하여 맵을 생성하는 팩맨 게임

## IsItemInLabels 함수 설명

```c
wchar_t items[3][100] = { L"Dog", L"Human", L"Person" }; // 맵에 표현될 수 있는 오브젝트 목록
/**

* 이미지에 존재하는 객체를 확인한 후 표현될 수 있는 오브젝트 목록 반환

* result: 표현할 수 있는 오브젝트 목록

* labels: 이미지에 존재하는 객체 목록
  */
  void IsItemInLabels(int* result, wchar_t* labels)
  {
    wchar_t* pwc;
    wchar_t* pt;

    pwc = wcstok(labels, L" ,", &pt);
    while (pwc != NULL)
    {
        for (int i = 0; i < 3; i++) {

  ​      if (wcsstr(pwc, items[i]) != NULL) {
  ​          result[i] = 1;
  ​      }
    }

    pwc = wcstok(NULL, L" ,", &pt);

    }

}
```




* pwc에는 라벨이 담긴다. 예를 들어 labels에 "People", "Dog", "Animal" 이 들어있다고 가정하자.
  그러면 pwc에는 labels에 들어있는 문자열들이 분리되어 담기게된다. 
  처음에는 "People" 그 다음에는 "Dog" 그 다음에는 "Animal"

* 분리된 라벨 데이터 pwc가 표현가능한 객체인지 확인하기 위해 items 배열에 있는 데이터와 비교한다.
  표현 가능한 객체가 있으면 1, 없으면 0이다. 따라서, 위와 같은 경우에는 result = {0,1,0}이 된다.

* result 배열을 사용하여 맵에 장애물이나 목표물, 몬스터를 생성할 수 있다. 예를 들면, result = {0,1,0} 이므로
  맵 생성시 표현가능한 오브젝트는 "Dog"이므로 맵에 Dog를 생성하면 된다.

## uploadImage 함수 설명

```c
memset(&OFN, 0, sizeof(OPENFILENAME));
OFN.lStructSize = sizeof(OPENFILENAME);
OFN.hwndOwner = hWnd;
OFN.lpstrFilter = filter;
OFN.lpstrFile = lpstrFile; // 파일의 경로
OFN.nMaxFile = 100;
OFN.lpstrInitialDir = _T(".");
OFN.lpstrFileTitle = lpstrFileTitle; // 파일명 저장
OFN.nMaxFileTitle = 100;

if (GetOpenFileName(&OFN) != 0) {
                    _stprintf_s(imageName, _T("/api/%s"), lpstrFileTitle); // label을 가져오기 위해 필요한 파일명 저장
                    _stprintf_s(imageNameUrl, _T("/api/uploads/%s"), lpstrFileTitle); // url을 가져오기 위해 필요한 파일명 저장
                    _stprintf_s(str, _T("%s 파일이 성공적으로 업로드 되었습니다."), lpstrFile);
              

                uploadImage(lpstrFile, imageNameUrl); // 이미지 업로드

               MessageBox(hWnd, str, _T("업로드 확인"), MB_OK);
}
```

* 위 코드를 보면 uploadImage 함수에는 두 가지 파라미터가 필요하다.
  첫번째는 이미지 파일의 절대경로, 두번째는 정제된 이미지 파일명

* 이미지가 정상적으로 업로드 되지 않으면 -1을 반환하고 정상적이면 0을 반환한다.

## getImageLabels 함수 설명

```c
getImageLabels(imageName, labels); // 라벨링 데이터 가져와 labels에 저장
```

* 위 함수는 매우 사용이 간단하다. 
  첫번째 인자는 이전에 이미지 업로드 하기전에 생성한 imageName을 사용
  두번째 인자는 이미지 객체 정보를 담기 위한 wchart_t 형태의 배열
  
  
